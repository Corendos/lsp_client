#include <lsp/client.h>
#include <lsp/reader.h>
#include <lsp/writer.h>

#include <reborn/strings.h>
#include <reborn/io.h>

ThreadData make_thread_data() {
    ThreadData thread_data = {0};
    
    thread_data.thread_ready_cv = make_condition_variable();
    thread_data.thread_mutex = make_mutex();
    
    return thread_data;
}

void destroy_thread_data(ThreadData* thread_data) {
    delete_mutex(&thread_data->thread_mutex);
    delete_condition_variable(&thread_data->thread_ready_cv);
}

void print_error(ConstStringU8 str, DWORD error_code) {
    static TCHAR format_buffer[1024] = {0};
    DWORD bytes_written = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code, 0, format_buffer, 1024, 0);
    if (bytes_written != 0) {
        println("%.*s %.*s", expand_string(str), bytes_written, format_buffer);
        return;
    }
    
    TCHAR* allocated_buffer = 0;
    
    // NOTE(Corentin): retry with an allocated buffer
    bytes_written = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, error_code, 0,
                                  (LPTSTR)&allocated_buffer, 1024, 0);
    
    if (bytes_written != 0) {
        println("%.*s %.*s", expand_string(str), bytes_written, allocated_buffer);
    } else {
        println("Failed to format message, error code was %d", error_code);
    }
    
    LocalFree(allocated_buffer);
}

bool create_pipes(LspClientContext* context) {
    SECURITY_ATTRIBUTES security_attrs = {0};
    security_attrs.nLength             = sizeof(SECURITY_ATTRIBUTES);
    security_attrs.bInheritHandle      = TRUE;
    
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

bool create_child_process(LspClientContext* context, ConstStringU8 executable) {
    STARTUPINFO startup_info = {0};
    BOOL success             = FALSE;
    
    startup_info.cb = sizeof(STARTUPINFO);
    // startup_info.hStdError = global_client_context.child_stdout_writer;
    startup_info.hStdOutput = context->child_stdout_writer;
    startup_info.hStdInput  = context->child_stdin_reader;
    startup_info.dwFlags |= STARTF_USESTDHANDLES;
    
    success = CreateProcess(executable.data, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startup_info,
                            &context->child_process_information);
    
    return success;
}

void wait_for_thread(ThreadData* thread_data) {
    acquire_mutex(&thread_data->thread_mutex);
    while (!thread_data->thread_ready) {
        wait_condition_variable(&thread_data->thread_ready_cv, &thread_data->thread_mutex);
    }
    release_mutex(&thread_data->thread_mutex);
}

bool initialize(LspClientContext* context, ConstStringU8 zls_executable) {
    context->reader_thread_data = make_thread_data();
    context->writer_thread_data = make_thread_data();
    
    context->request_hash_table_mutex = make_mutex();
    context->request_hash_table_allocator = make_arena_allocator(0);
    context->request_hash_table = make_request_hash_table(&context->request_hash_table_allocator, 64);
    
    context->client_process_id = GetCurrentProcessId();
    if(!create_pipes(context)) {
        println("Failed to create pipe");
        return false;
    }
    
    // Create child process
    if (!create_child_process(context, zls_executable)) {
        print_error(SCu8("Failed to create child process:"), GetLastError());
        return false;
    }
    
    context->reader_thread_data.thread = make_thread(SCu8("reader_thread"), reader_thread_function, context);
    context->writer_thread_data.thread = make_thread(SCu8("writer_thread"), writer_thread_function, context);
    
    wait_for_thread(&context->reader_thread_data);
    wait_for_thread(&context->writer_thread_data);
    
    return true;
}

void shutdown(LspClientContext* context) {
    // TODO(Corentin): handle shutdown
    TerminateProcess(context->child_process_information.hProcess, 0);
    CancelSynchronousIo(context->writer_thread_data.thread.native_handle.handle);
    CancelSynchronousIo(context->reader_thread_data.thread.native_handle.handle);
    
    join_thread(&context->reader_thread_data.thread);
    join_thread(&context->writer_thread_data.thread);
    
    destroy_request_hash_table(&context->request_hash_table_allocator, &context->request_hash_table);
    destroy_thread_data(&context->reader_thread_data);
    destroy_thread_data(&context->writer_thread_data);
}


MessageProcessingMap make_message_processing_map(Allocator* allocator, u64 capacity) {
    MessageProcessingMap map = {0, 0, capacity};
    map.entries = (MessageProcessingMapEntry*)allocate(allocator, sizeof(MessageProcessingMapEntry) * capacity);
    return map;
}

bool push_message_processing_handlers(MessageProcessingMap* map, MessageProcessingMapEntry entry) {
    if (map->size >= map->capacity) return false;
    map->entries[map->size] = entry;
    map->size++;
    return true;
}

bool push_message_processing_handlers(MessageProcessingMap* map, ConstStringU8 method,
                                      MessageProcessingHandlers handlers) {
    return push_message_processing_handlers(map, MessageProcessingMapEntry{method, handlers});
}

MessageProcessingHandlers* lookup_message_processing_handler(MessageProcessingMap* map, ConstStringU8 method) {
    for (u64 i = 0; i < map->size; ++i) {
        MessageProcessingMapEntry* entry = map->entries + i;
        if (string_compare(entry->method, method) == 0) {
            return &entry->handlers;
        }
    }
    
    return 0;
}
