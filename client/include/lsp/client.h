#ifndef LSP_CLIENT_H
#define LSP_CLIENT_H

#include <lsp/types.h>
#include <lsp/request_hash_table.h>

#include <reborn/concurrency/thread.h>
#include <reborn/concurrency/mutex.h>
#include <reborn/concurrency/condition_variable.h>

void print_error(ConstStringU8 str, DWORD error_code);

struct ThreadData {
    Thread thread;
    
    Mutex thread_mutex;
    ConditionVariable thread_ready_cv;
    bool thread_ready;
};

struct MessageHandlers {
    void* (*allocation_function)(Allocator* /* allocator */, void* /* user_data */);
    bool (*parsing_function)(Allocator* /* allocator */, VALUE* /* json */, void* /* user_data */, void* /* opaque_type */);
    void (*processing_function)(Allocator* /* allocator */, void* /* user_data */, void* /* opaque_type */);
    void* user_data;
};

struct NotificationHandlersMap {
    MessageHandlers handlers[ServerNotificationType_COUNT];
};

void set_notification_handlers(NotificationHandlersMap* map, ServerNotificationType type, MessageHandlers handlers);
MessageHandlers get_notification_handlers(NotificationHandlersMap* map, ServerNotificationType type);

struct RequestHandlersMap {
    MessageHandlers handlers[ServerRequestType_COUNT];
};

void set_request_handlers(RequestHandlersMap* map, ServerRequestType type, MessageHandlers handlers);
MessageHandlers get_request_handlers(RequestHandlersMap* map, ServerRequestType type);

struct ResponseHandlersMap {
    MessageHandlers handlers[ClientRequestType_COUNT];
};

void set_response_handlers(ResponseHandlersMap* map, ClientRequestType type, MessageHandlers handlers);
MessageHandlers get_response_handlers(ResponseHandlersMap* map, ClientRequestType type);

struct LspClientContext {
    HANDLE child_stdin_reader;
    HANDLE child_stdin_writer;
    HANDLE child_stdout_reader;
    HANDLE child_stdout_writer;
    PROCESS_INFORMATION child_process_information;
    DWORD client_process_id;
    
    ThreadData reader_thread_data;
    ThreadData writer_thread_data;
    
    Mutex request_hash_table_mutex;
    RequestHashTable request_hash_table;
    Allocator request_hash_table_allocator;
    
    Allocator allocator;
    
    NotificationHandlersMap notification_handlers_map;
    RequestHandlersMap request_handlers_map;
    ResponseHandlersMap response_handlers_map;
};

bool initialize(LspClientContext* context, ConstStringU8 zls_executable);
void shutdown(LspClientContext* context);







#endif //LSP_CLIENT_H
