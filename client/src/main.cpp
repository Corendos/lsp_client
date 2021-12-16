#include <json/json-dom.h>
#include <json/value.h>

#include <windows.h>
#include <signal.h>

#include <std/strings.h>

#include <lsp/types.h>

#include <cstdint>
#include <cstdio>
#include <cassert>

#define BUFFER_SIZE 4096

struct Iterator {
    char *data;
    char *current;
    u64 size;
};

Iterator make_iterator(char *data, u64 size) {
    Iterator it = {data, data, size};
    return it;
}

Iterator make_iterator(StringU8 *str) {
    return make_iterator(str->data, str->size);
}

inline u64 get_position(Iterator *it) {
    return it->current - it->data;
}

inline bool is_valid(Iterator *it) {
    return (it->current - it->data) < it->size;
}

inline void skip_to(Iterator* it, char c) {
    if (is_valid(it) && it->current[0] == c) return;
    while (is_valid(it)) {
        it->current++;
        if (it->current[0] == c) return;
    }
}

inline void skip_to_first_after(Iterator* it, char c) {
    skip_to(it, c);
    it->current++;
}

inline bool is_number(char c) {
    return c >= '0' && c <= '9';
}

inline void skip_to_first_number(Iterator* it) {
    if (is_valid(it) && is_number(it->current[0])) return;
    while (is_valid(it)) {
        it->current++;
        if (is_number(it->current[0])) return;
    }
}

inline char peek(Iterator* it, u64 offset) {
    return (it->current + offset - it->data) >= it->size ? 0 : it->current[offset];
}

inline u64 parse_u64(Iterator *it) {
    u64 value = 0;
    while (is_valid(it) && is_number(it->current[0])) {
        value = 10 * value + it->current[0] - '0';
        it->current++;
    }
    return value;
}

inline u64 parse_u64(ConstStringU8 *str) {
    u64 value = 0;
    for (u64 i = 0;i < str->size;++i) {
        char c = str->data[i];
        if (!is_number(c)) {
            break;
        }
        value = 10 * value + c - '0';
    }
    return value;
}


struct Thread {
    DWORD id;
    HANDLE handle;
};

struct ClientContext {
    HANDLE child_stdin_reader;
    HANDLE child_stdin_writer;
    HANDLE child_stdout_reader;
    HANDLE child_stdout_writer;
    PROCESS_INFORMATION child_process_information;
    Thread reader_thread;
    Thread writer_thread;
};

static ClientContext global_client_context = {0};

void print_error(ConstStringU8 str, DWORD error_code) {
    static TCHAR format_buffer[1024] = {0};
    DWORD bytes_written = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code, 0, format_buffer, 1024, 0);
    if (bytes_written != 0) {
        printf("%.*s %.*s\n", expand_string(str), bytes_written, format_buffer);
        return;
    }
    
    TCHAR* allocated_buffer = 0;
    
    // NOTE(Corentin): retry with an allocated buffer
    bytes_written = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                  0,
                                  error_code,
                                  0,
                                  (LPTSTR)&allocated_buffer,
                                  1024,
                                  0);
    
    if (bytes_written != 0) {
        printf("%.*s %.*s\n", expand_string(str), bytes_written, allocated_buffer);
    } else {
        printf("Failed to format message, error code was %d\n", error_code);
    }
    
    LocalFree(allocated_buffer);
}

bool create_pipes(ClientContext *context) {
    SECURITY_ATTRIBUTES security_attrs = {0};
    security_attrs.nLength = sizeof(SECURITY_ATTRIBUTES);
    security_attrs.bInheritHandle = TRUE;
    
    if (!CreatePipe(&context->child_stdout_reader, &context->child_stdout_writer, &security_attrs, 0)) {
        print_error(SCu8("Error creating stdout pipe:"), GetLastError());
        return false;
    }
    
    if (!SetHandleInformation(context->child_stdout_reader, HANDLE_FLAG_INHERIT, 0)) {
        print_error(SCu8("Error configuring stdout pipe:"), GetLastError());
        return false;
    }
    
    if (!CreatePipe(&context->child_stdin_reader, &context->child_stdin_writer, &security_attrs, 0)) {
        print_error(SCu8("Error creating stdin pipe:"), GetLastError());
        return false;
    }
    
    if (!SetHandleInformation(context->child_stdin_writer, HANDLE_FLAG_INHERIT, 0)) {
        print_error(SCu8("Error configuring stdin pipe:"), GetLastError());
        return false;
    }
    
    return true;
}

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
    Arena storage;
};

ParsingContext make_parsing_context() {
    ParsingContext context = {0};
    
    context.state = ParsingState_Headers;
    context.storage = make_arena(KB(4));
    context.json_buffer = push_string(&context.storage, 1024);
    context.temp_header_buffer = push_string(&context.storage, 256);
    
    return context;
}

void reset_parsing_context(ParsingContext *context) {
    context->state = ParsingState_Headers;
    context->header_count = 0;
    reset_string(&context->json_buffer);
    reset_string(&context->temp_header_buffer);
}

void free_parsing_context(ParsingContext *context) {
    reset_parsing_context(context);
    free_arena(&context->storage);
}

Header *get_current_header(ParsingContext *context) {
    if (context->header_count < 16) {
        return context->headers + context->header_count;
    }
    return 0;
}

Header make_header_from_buffer(Arena *arena, StringU8 *buffer, i64 comma_pos, i64 value_pos) {
    Header header = {
        push_const_string(arena, buffer->data, comma_pos),
        push_const_string(arena, buffer->data + value_pos, buffer->size - value_pos)
    };
    return header;
}

void parse_header(Iterator *it, ParsingContext *context) {
    while (is_valid(it) && it->current[0] != '\r') {
        write_to(&context->temp_header_buffer, it->current[0]);
        it->current++;
    }
    
    Iterator header_it = make_iterator(&context->temp_header_buffer);
    i64 comma_position = -1;
    i64 value_position = -1;
    
    while (is_valid(&header_it)) {
        if (comma_position == -1 && header_it.current[0] == ':') {
            comma_position = get_position(&header_it);
        } else if (comma_position != -1 && header_it.current[0] != ' ') {
            value_position = get_position(&header_it);
            break;
        }
        header_it.current++;
    }
    
    Header *current = get_current_header(context);
    if (current) {
        *current = make_header_from_buffer(&context->storage, &context->temp_header_buffer, comma_position, value_position);
        context->header_count++;
    }
    reset_string(&context->temp_header_buffer);
}

u64 get_content_length_from_headers(Header *headers, u64 count) {
    for (u64 i = 0;i < count;++i) {
        Header *current = headers + i;
        if (string_compare(current->name, SCu8("Content-Length")) == 0) {
            return parse_u64(&current->value);
        }
    }
    return 0;
}

bool feed_parser(Iterator *it, ParsingContext *context) {
    while (is_valid(it)) {
        if (context->state == ParsingState_Headers) {
            // NOTE(Corentin): if we have an empty line, we need to go to the next line
            if (it->current[0] == '\r') {
                context->state = ParsingState_ContentNextLine;
                continue;
            }
            
            // Otherwise, we try to parse the current header
            parse_header(it, context);
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
            assert(context->content_length != 0);
            while (is_valid(it)) {
                write_to(&context->json_buffer, it->current[0]);
                it->current++;
                if (context->json_buffer.size == context->content_length) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

struct MessageProcessingHandlers {
    void* (*allocation_function)(Arena* /* arena */, void* /* user_data */);
    bool (*parsing_function)(Arena* /* arena */, VALUE* /* json */, void* /* user_data */, void* /* opaque_type */);
    void (*processing_function)(Arena* /* arena */, void* /* user_data */, void* /* opaque_type */);
    void* user_data;
};

struct MessageProcessingMapEntry {
    ConstStringU8 method;
    MessageProcessingHandlers handlers;
};

struct MessageProcessingMap {
    MessageProcessingMapEntry *entries;
    u64 size;
    u64 capacity;
};

MessageProcessingMap make_message_processing_map(Arena* storage, u64 capacity) {
    MessageProcessingMap map = {0, 0, capacity};
    map.entries = (MessageProcessingMapEntry*)allocate(storage, sizeof(MessageProcessingMapEntry) * capacity);
    return map;
}

bool push_message_processing_handlers(MessageProcessingMap* map, MessageProcessingMapEntry entry) {
    if (map->size >= map->capacity) return false;
    map->entries[map->size] = entry;
    map->size++;
    return true;
}

bool push_message_processing_handlers(MessageProcessingMap* map, ConstStringU8 method, MessageProcessingHandlers handlers) {
    return push_message_processing_handlers(map, MessageProcessingMapEntry{method, handlers});
}

MessageProcessingHandlers *lookup_message_processing_handler(MessageProcessingMap* map, ConstStringU8 method) {
    for (u64 i = 0;i < map->size;++i) {
        MessageProcessingMapEntry *entry = map->entries + i;
        if (string_compare(entry->method, method) == 0) {
            return &entry->handlers;
        }
    }
    
    return 0;
}

void* allocate_log_message_command(Arena *arena, void *user_data) {
    return allocate(arena, sizeof(LogMessageParams));
}

void process_log_message_command(Arena *arena, void *user_data, void *opaque_type) {
    if (opaque_type == 0) return;
    LogMessageParams* log_message_params = (LogMessageParams*)opaque_type;
    
    ConstStringU8 type_str = to_string(log_message_params->type);
    printf("%.*s %.*s\n", expand_string(type_str), expand_string(log_message_params->message));
}

bool parse_log_message_params(Arena *arena, VALUE *json, void* user_data, void* opaque_type) {
    if (!opaque_type) return false;
    
    LogMessageParams* log_message_params = (LogMessageParams*)opaque_type;
    
    if (value_type(json) != VALUE_DICT) return false;
    
    VALUE *type_value = value_path(json, "type");
    if (!type_value || !value_is_compatible(type_value, VALUE_UINT64)) return false;
    log_message_params->type = value_uint64(type_value);
    
    VALUE *message_value = value_path(json, "message");
    if (!message_value || value_type(message_value) != VALUE_STRING) return false;
    log_message_params->message = push_const_string(arena, value_string(message_value), value_string_length(message_value));
    
    return true;
}

bool parse_notification_message(Arena* arena, VALUE *json, NotificationMessage *message) {
    VALUE *method_value = value_path(json, "method");
    if (!method_value || value_type(method_value) != VALUE_STRING) return false;
    message->method = push_const_string(arena, value_string(method_value), (u64)value_string_length(method_value));
    
    VALUE *params_value = value_path(json, "params");
    if (params_value && (value_type(params_value) != VALUE_ARRAY && value_type(params_value) != VALUE_DICT)) return false;
    message->params = params_value;
    
    return true;
}

void handle_notification_message(MessageProcessingMap *map, Arena* arena, VALUE *json) {
    NotificationMessage message = {0};
    
    if (!parse_notification_message(arena, json, &message)) {
        printf("Failed to parse NotificationMessage\n");
        return;
    }
    
    MessageProcessingHandlers *handlers = lookup_message_processing_handler(map, message.method);
    if (!handlers) {
        printf("No handlers to handle the method '%.*s'\n", expand_string(message.method));
        return;
    }
    
    if (!handlers->allocation_function) {
        printf("No allocation function command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    void *command_storage = handlers->allocation_function(arena, 0);
    
    if (!handlers->parsing_function) {
        printf("No parsing function for command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    bool parsing_success = handlers->parsing_function(arena, message.params, 0, command_storage);
    if (!parsing_success) {
        printf("Failed to parse params for command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    if (!handlers->processing_function) {
        printf("No processing function for command '%.*s'\n", expand_string(message.method));
        return;
    }
    
    handlers->processing_function(arena, 0, command_storage);
}

void handle_inbound_request(MessageProcessingMap *map, Arena* arena, VALUE *json) {
    if (value_path(json, "method")) {
        // Process NotificationMessage
        handle_notification_message(map, arena, json);
    } else {
        // Process ResponseMessage
    }
}

DWORD WINAPI reader_thread_function(void *parameters) {
    DWORD bytesRead = 0;
    char buffer[BUFFER_SIZE] = {0};
    
    ParsingContext context = make_parsing_context();
    
    Arena inbound_request_arena = make_arena();
    Arena reader_thread_arena = make_arena();
    
    MessageProcessingMap message_processing_map = make_message_processing_map(&reader_thread_arena, 64);
    
    MessageProcessingHandlers log_message_handlers = {
        allocate_log_message_command,
        parse_log_message_params,
        process_log_message_command,
        0
    };
    
    push_message_processing_handlers(&message_processing_map, SCu8("window/logMessage"), log_message_handlers);
    
    while (1) { 
        BOOL success = ReadFile(global_client_context.child_stdout_reader, buffer, BUFFER_SIZE, &bytesRead, NULL);
        if(!success || bytesRead == 0) break;
        
        Iterator it = make_iterator(buffer, bytesRead);
        
        bool request_ready = feed_parser(&it, &context);
        
        if (request_ready) {
            // extract command
            VALUE json_root = {0};
            
            if (json_dom_parse(context.json_buffer.data, context.json_buffer.size, 0, 0, &json_root, 0) != 0) {
                printf("Failed to decode JSON\n");
                continue;
            }
            
            handle_inbound_request(&message_processing_map, &inbound_request_arena, &json_root);
            
            value_fini(&json_root);
            reset_arena(&inbound_request_arena);
        }
        
        reset_parsing_context(&context);
    }
    
    printf("Exiting reader thread\n");
    free_arena(&inbound_request_arena);
    free_arena(&reader_thread_arena);
    return 0;
}

DWORD WINAPI writer_thread_function(void *parameters) {
    return 0;
}

bool create_thread(Thread* thread, DWORD (WINAPI *function_pointer)(void *)) {
    SECURITY_ATTRIBUTES security_attrs = {0};
    security_attrs.nLength = sizeof(SECURITY_ATTRIBUTES);
    security_attrs.bInheritHandle = FALSE;
    
    DWORD thread_id = 0;
    thread->handle = CreateThread(&security_attrs, 0, function_pointer, 0, 0, &thread->id);
    if (thread->handle == NULL) {
        print_error(SCu8("Failed to create thread:"), GetLastError());
        return false;
    }
    
    return true;
}

struct SignalContext {
    void (*old_sigint_handler)(int);
};

static SignalContext global_signal_context = {0};

void sigint_handler(int signal_number) {
    if (signal_number == SIGINT) {
        TerminateProcess(global_client_context.child_process_information.hProcess, 0);
        CancelSynchronousIo(global_client_context.writer_thread.handle);
        CancelSynchronousIo(global_client_context.reader_thread.handle);
    }
    
    if (global_signal_context.old_sigint_handler != 0) (*global_signal_context.old_sigint_handler)(signal_number);
}

void install_signal_handler(int signal_number, void (*handler)(int)) {
    global_signal_context.old_sigint_handler = signal(signal_number, handler);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Error, expected 2 arguments, got %d\n", argc);
        return 1;
    }
    
    StringU8 zls_executable_path = make_string(argv[1]);
    
    create_pipes(&global_client_context);
    
    // Create child process
    STARTUPINFO startup_info = {0};
    BOOL success = FALSE;
    
    startup_info.cb = sizeof(STARTUPINFO);
    startup_info.hStdError = global_client_context.child_stdout_writer;
    startup_info.hStdOutput = global_client_context.child_stdout_writer;
    startup_info.hStdInput = global_client_context.child_stdin_reader;
    startup_info.dwFlags |= STARTF_USESTDHANDLES;
    
    success = CreateProcess(NULL,
                            zls_executable_path.data,
                            NULL,
                            NULL,
                            TRUE,
                            0,
                            NULL,
                            NULL,
                            &startup_info,
                            &global_client_context.child_process_information);
    
    install_signal_handler(SIGINT, sigint_handler);
    
    if (!success) {
        print_error(SCu8("Failed to create child process:"), GetLastError());
        return 1;
    }
    
    if (!create_thread(&global_client_context.reader_thread, reader_thread_function)) {
        return 1;
    }
    
    if (!create_thread(&global_client_context.writer_thread, writer_thread_function)) {
        return 1;
    }
    
    WaitForSingleObject(global_client_context.reader_thread.handle, INFINITE);
    WaitForSingleObject(global_client_context.writer_thread.handle, INFINITE);
    
    return 0;
}