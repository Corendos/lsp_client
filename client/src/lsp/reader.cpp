#include <lsp/reader.h>
#include <lsp/types.h>
#include <lsp/client.h>

#include <reborn/memory/allocator.h>
#include <reborn/strings.h>
#include <reborn/iterator.h>
#include <reborn/io.h>

#include <json/json-dom.h>
#include <json/value.h>

#define BUFFER_SIZE 4096

typedef u32 ParsingState;
enum {
    ParsingState_Headers,
    ParsingState_HeadersNextLine,
    ParsingState_Content,
    ParsingState_ContentNextLine,
};

struct Header {
    ConstStringU8 name;
    ConstStringU8 value;
};

struct ParsingContext {
    ParsingState state;
    u64 content_length;
    
    Header headers[16];
    u32 header_count;
    StringU8 temp_header_buffer;
    
    StringU8 json_buffer;
    Allocator allocator;
};

ParsingContext make_parsing_context() {
    ParsingContext context = {0};
    
    context.state              = ParsingState_Headers;
    context.allocator          = make_arena_allocator(0);
    context.json_buffer        = push_string_u8(&context.allocator, 4096);
    context.temp_header_buffer = push_string_u8(&context.allocator, 256);
    
    return context;
}

void reset_parsing_context(ParsingContext* context) {
    context->state        = ParsingState_Headers;
    context->header_count = 0;
    clear_string(&context->json_buffer);
    clear_string(&context->temp_header_buffer);
}

void free_parsing_context(ParsingContext* context) {
    reset_parsing_context(context);
    destroy_arena_allocator(&context->allocator);
}

Header* get_current_header(ParsingContext* context) {
    if (context->header_count < 16) {
        return context->headers + context->header_count;
    }
    return 0;
}

Header make_header_from_buffer(Allocator* allocator, StringU8* buffer, i64 colon_pos, i64 value_pos) {
    Header header = {
        push_const_string(allocator, buffer->data, colon_pos),
        push_const_string(allocator, buffer->data + value_pos, buffer->size - value_pos)
    };
    return header;
}

bool parse_header(Iterator* it, ParsingContext* context) {
    while (is_valid(it) && it->current[0] != '\r') {
        write_to(&context->temp_header_buffer, it->current[0]);
        it->current++;
    }
    
    Iterator header_it = make_iterator(&context->temp_header_buffer);
    i64 colon_position = -1;
    i64 value_position = -1;
    
    while (is_valid(&header_it)) {
        if (colon_position == -1 && header_it.current[0] == ':') {
            colon_position = get_position(&header_it);
        } else if (colon_position != -1 && header_it.current[0] != ' ') {
            value_position = get_position(&header_it);
            break;
        }
        header_it.current++;
    }
    
    bool result = false;
    
    if (colon_position != -1 && value_position != -1) {
        result          = true;
        Header* current = get_current_header(context);
        if (current) {
            *current = make_header_from_buffer(&context->allocator, &context->temp_header_buffer, colon_position,
                                               value_position);
            context->header_count++;
        }
    }
    
    clear_string(&context->temp_header_buffer);
    return result;
}

u64 get_content_length_from_headers(Header* headers, u64 count) {
    for (u64 i = 0; i < count; ++i) {
        Header* current = headers + i;
        if (string_compare(current->name, SCu8("Content-Length")) == 0) {
            return parse_u64(&current->value);
        }
    }
    return 0;
}

typedef u8 ParserResult;
enum { ParserResult_RequestReady, ParserResult_Buffering, ParserResult_Error };

ParserResult feed_parser(Iterator* it, ParsingContext* context) {
    while (is_valid(it)) {
        if (context->state == ParsingState_Headers) {
            // NOTE(Corentin): if we have an empty line, we need to go to the next line
            if (it->current[0] == '\r') {
                context->state = ParsingState_ContentNextLine;
                continue;
            }
            
            // Otherwise, we try to parse the current header
            if (!parse_header(it, context)) {
                return ParserResult_Error;
            }
            context->state = ParsingState_HeadersNextLine;
            
        } else if (context->state == ParsingState_HeadersNextLine) {
            // We loop until we find a line feed or the iterator is invalid. If we find a line feed,
            // we start to look for a header
            while (is_valid(it)) {
                if (it->current[0] == '\n') {
                    it->current++;
                    context->state = ParsingState_Headers;
                    break;
                }
                it->current++;
            }
        } else if (context->state == ParsingState_ContentNextLine) {
            // We loop until we find a line feed or the iterator is invalid. If we find a line feed,
            // we start to look for the actual content
            while (is_valid(it)) {
                if (it->current[0] == '\n') {
                    it->current++;
                    context->state = ParsingState_Content;
                    break;
                }
                it->current++;
            }
        } else if (context->state == ParsingState_Content) {
            context->content_length = get_content_length_from_headers(context->headers, context->header_count);
            // If we don't know the content length, abort
            ASSERT(context->content_length != 0);
            while (is_valid(it)) {
                ASSERT(write_to(&context->json_buffer, it->current[0]));
                it->current++;
                if (context->json_buffer.size == context->content_length) {
                    return ParserResult_RequestReady;
                }
            }
        }
    }
    
    return ParserResult_Buffering;
}

void* allocate_log_message_command(Allocator* allocator, void* user_data) { return allocate(allocator, sizeof(LogMessageParams)); }

void process_log_message_command(Allocator* allocator, void* user_data, void* opaque_type) {
    if (opaque_type == 0) return;
    LogMessageParams* log_message_params = (LogMessageParams*)opaque_type;
    
    ConstStringU8 type_str = to_string(log_message_params->type);
    print("%.*s %.*s\n", expand_string(type_str), expand_string(log_message_params->message));
}

bool parse_log_message_params(Allocator* allocator, VALUE* json, void* user_data, void* opaque_type) {
    if (!opaque_type) return false;
    
    LogMessageParams* log_message_params = (LogMessageParams*)opaque_type;
    
    if (value_type(json) != VALUE_DICT) return false;
    
    VALUE* type_value = value_path(json, "type");
    if (!type_value || !value_is_compatible(type_value, VALUE_UINT64)) return false;
    log_message_params->type = MessageType{(u8)value_uint64(type_value)};
    
    VALUE* message_value = value_path(json, "message");
    if (!message_value || value_type(message_value) != VALUE_STRING) return false;
    log_message_params->message =
        push_const_string(allocator, value_string(message_value), value_string_length(message_value));
    
    return true;
}

bool parse_notification_message(Allocator* allocator, VALUE* json, NotificationMessage* message) {
    VALUE* method_value = value_path(json, "method");
    if (!method_value || value_type(method_value) != VALUE_STRING) return false;
    message->method = push_const_string(allocator, value_string(method_value), (u64)value_string_length(method_value));
    
    VALUE* params_value = value_path(json, "params");
    if (params_value && (value_type(params_value) != VALUE_ARRAY && value_type(params_value) != VALUE_DICT))
        return false;
    message->params = params_value;
    
    return true;
}

void handle_notification_message(LspClientContext* client_context,
                                 MessageProcessingMap* map,
                                 Allocator* allocator,
                                 VALUE* json) {
    NotificationMessage message = {0};
    
    if (!parse_notification_message(allocator, json, &message)) {
        print("Failed to parse NotificationMessage\n");
        return;
    }
    
    MessageProcessingHandlers* handlers = lookup_message_processing_handler(map, message.method);
    if (!handlers) {
        print("No handlers to handle the method '%.*s'\n", expand_string(message.method));
        return;
    }
    
    if (!handlers->allocation_function) {
        print("No allocation function command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    void* command_storage = handlers->allocation_function(allocator, 0);
    
    if (!handlers->parsing_function) {
        print("No parsing function for command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    bool parsing_success = handlers->parsing_function(allocator, message.params, 0, command_storage);
    if (!parsing_success) {
        print("Failed to parse params for command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    if (!handlers->processing_function) {
        print("No processing function for command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    handlers->processing_function(allocator, 0, command_storage);
}

bool parse_response_error(Allocator* allocator, VALUE* json, ResponseError* response_error) {
    VALUE* code_value = value_path(json, "code");
    if (!code_value || !value_is_compatible(code_value, VALUE_INT64)) {
        return false;
    }
    
    response_error->code.value = value_int64(code_value);
    
    VALUE* message_value = value_path(json, "message");
    if (!message_value || value_type(message_value) != VALUE_STRING) {
        return false;
    }
    
    response_error->message =
        push_const_string(allocator, value_string(message_value), (u64)value_string_length(message_value));
    response_error->data = value_path(json, "data");
    
    return true;
}

bool parse_response_message(Allocator* allocator, VALUE* json, ResponseMessage* message) {
    VALUE* id_value = value_path(json, "id");
    if (!id_value) {
        return false;
    }
    
    if (value_is_compatible(id_value, VALUE_UINT64)) {
        message->id.type.value = ResponseMessageIdType_Integer;
        message->id.number     = value_uint64(id_value);
    } else if (value_type(id_value) == VALUE_STRING) {
        message->id.type.value = ResponseMessageIdType_String;
        message->id.str        = push_const_string(allocator, value_string(id_value), (u64)value_string_length(id_value));
    } else if (value_type(id_value) == VALUE_NULL) {
        message->id.type.value = ResponseMessageIdType_Null;
    } else {
        return false;
    }
    
    message->result = value_path(json, "result");
    
    VALUE* error_value = value_path(json, "error");
    if (error_value) {
        message->error.has_value = true;
        if (!parse_response_error(allocator, error_value, &message->error.value)) {
            return false;
        }
    }
    
    return true;
}

void handle_response_message(LspClientContext* client_context,
                             Allocator* allocator,
                             VALUE* json) {
    ResponseMessage message = {0};
    
    if (!parse_response_message(allocator, json, &message)) {
        print("Failed to parse ResponseMessage\n");
        return;
    }
    
    println("Parsed ResponseMessage !");
    
    if (message.id.type.value != ResponseMessageIdType_Integer) {
        println("ResponseMessage id is not an integer");
        return;
    }
    
    acquire_mutex(&client_context->request_hash_table_mutex);
    RequestHashTableLookupResult request_type_result = 
        lookup_request_hash_table(&client_context->request_hash_table,
                                  message.id.number);
    release_mutex(&client_context->request_hash_table_mutex);
    
    if (!request_type_result.found) {
        println("No request found associated with id %d", message.id.number);
        return;
    }
    
    //if ()
    
    println("Request with id %d was a %d", message.id.number, request_type_result.request_type);
    
    return;
}

void handle_server_message(LspClientContext* client_context,
                           Allocator* allocator,
                           VALUE* json) {
    bool has_id = value_path(json, "id");
    bool has_method = value_path(json, "method");
    
    if (has_id) {
        if (has_method) {
            // TODO(Corentin): handle request
        } else {
            handle_response_message(client_context, map, allocator, json);
        }
    } else if (has_method) {
        handle_notification_message(client_context, map, allocator, json);
    } else {
        println("Unknown message type received");
    }
}

u64 reader_thread_function(void* parameters) {
    LspClientContext* client_context = (LspClientContext*)parameters;
    
    DWORD bytesRead          = 0;
    char buffer[BUFFER_SIZE] = {0};
    
    ParsingContext context = make_parsing_context();
    
    // TODO(Corentin): make that a scratch buffer someday
    Arena request_arena = make_arena(KB(4));
    
    Allocator reader_thread_allocator = make_arena_allocator(0);
    
    MessageProcessingHandlers log_message_handlers = {allocate_log_message_command, parse_log_message_params,
        process_log_message_command, 0};
    
    push_message_processing_handlers(&message_processing_map, SCu8("window/logMessage"), log_message_handlers);
    
    acquire_mutex(&client_context->reader_thread_data.thread_mutex);
    client_context->reader_thread_data.thread_ready = true;
    release_mutex(&client_context->reader_thread_data.thread_mutex);
    notify_one_condition_variable(&client_context->reader_thread_data.thread_ready_cv);
    
    while (1) {
        BOOL success = ReadFile(client_context->child_stdout_reader, buffer, BUFFER_SIZE, &bytesRead, NULL);
        if (!success || bytesRead == 0) break;
        
        Iterator it = make_iterator(buffer, bytesRead);
        
        ParserResult parser_result = feed_parser(&it, &context);
        
        if (parser_result == ParserResult_Buffering) {
            continue;
        } else if (parser_result == ParserResult_Error) {
            print("Failed to parse incoming request\n");
            print("Received (%lu): %.*s\n", bytesRead, bytesRead, buffer);
            reset_parsing_context(&context);
            continue;
        }
        
        Allocator request_allocator = make_temporary_arena_allocator(&request_arena);
        
        // extract command
        VALUE json_root = {0};
        
        if (json_dom_parse(context.json_buffer.data, context.json_buffer.size, 0, 0, &json_root, 0) == 0) {
            handle_server_message(client_context, &request_allocator, &json_root);
            
            value_fini(&json_root);
        } else {
            print("Failed to decode JSON\n");
        }
        
        destroy_temporary_arena_allocator(&request_allocator);
        reset_parsing_context(&context);
    }
    
    print("Exiting reader thread\n");
    destroy_arena(&inbound_request_arena);
    destroy_arena_allocator(&reader_thread_allocator);
    free_parsing_context(&context);
    return 0;
}
