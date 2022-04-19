#include <lsp/writer.h>
#include <lsp/types.h>
#include <lsp/client.h>

#include <reborn/memory/allocator.h>
#include <reborn/strings.h>

#include <json/json-dom.h>
#include <json/value.h>

struct JsonDumpCallbackContext {
    StringU8 buffer;
};

int json_dump_callback(const char* str, size_t size, void* user_data) {
    JsonDumpCallbackContext* context = (JsonDumpCallbackContext*)user_data;
    
    format_to(&context->buffer, "%.*s", (int)size, str);
    
    return 0;
}

InitializeParams make_initialize_parameters(i64 process_id) {
    InitializeParams params = {0};
    
    params.process_id.has_value = true;
    params.process_id.value     = process_id;
    
    params.client_info.has_value               = true;
    params.client_info.value.name              = SCu8("4coder_lsp_client");
    params.client_info.value.version.has_value = true;
    params.client_info.value.version.value     = SCu8("0.0.1-dev");
    
    params.locale.has_value = true;
    params.locale.value     = SCu8("en-us");
    
    return params;
}

void send_initialization_request(LspClientContext* client_context) {
    Allocator allocator = make_arena_allocator(0);
    
    JsonDumpCallbackContext context = {0};
    context.buffer                  = push_string_u8(&allocator, 4096);
    
    
    InitializeParams params = make_initialize_parameters(client_context->client_process_id);
    
    
    VALUE request_json = {0};
    value_init_dict(&request_json);
    
    VALUE* id_value = value_dict_add(&request_json, "id");
    value_init_uint32(id_value, 1);
    
    VALUE* method_value = value_dict_add(&request_json, "method");
    value_init_string(method_value, "initialize");
    
    VALUE* params_value  = value_dict_add(&request_json, "params");
    *params_value = to_json(&params);
    
    json_dom_dump(&request_json, json_dump_callback, &context, 0, JSON_DOM_DUMP_INDENTWITHSPACES);
    
    StringU8 buffer = push_string_u8(&allocator, 4096);
    
    format_to(&buffer,
              "Content-Length: %llu\r\n"
              "\r\n"
              "%.*s",
              context.buffer.size, expand_string(context.buffer));
    
    acquire_mutex(&client_context->request_hash_table_mutex);
    add_request_hash_table(&client_context->request_hash_table_allocator,
                           &client_context->request_hash_table,
                           1,
                           ClientRequestType_Initialize);
    release_mutex(&client_context->request_hash_table_mutex);
    
    // print("Sending: %.*s\n", expand_string(buffer));
    
    DWORD bytes_written = 0;
    BOOL success = WriteFile(client_context->child_stdin_writer, buffer.data, buffer.size, &bytes_written, NULL);
    if (!success || bytes_written == 0) {
        print_error(SCu8("Failed to write to pipe:"), GetLastError());
    }
    
    value_fini(&request_json);
}


u64 writer_thread_function(void* parameters) {
    LspClientContext* client_context = (LspClientContext*)parameters;
    
    Allocator allocator = make_arena_allocator(0);
    
    Sleep(100);
    
    acquire_mutex(&client_context->writer_thread_data.thread_mutex);
    client_context->writer_thread_data.thread_ready = true;
    release_mutex(&client_context->writer_thread_data.thread_mutex);
    notify_one_condition_variable(&client_context->writer_thread_data.thread_ready_cv);
    
    send_initialization_request(client_context);
    
    destroy_arena_allocator(&allocator);
    return 0;
}
