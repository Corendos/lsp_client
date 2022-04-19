#include <json/json-dom.h>
#include <json/value.h>

#include <signal.h>
#include <windows.h>

#include <reborn/iterator.h>
#include <reborn/strings.h>
#include <reborn/io.h>
#include <reborn/concurrency/thread.h>
#include <reborn/concurrency/mutex.h>
#include <reborn/concurrency/condition_variable.h>

#include <lsp/client.h>

struct SignalContext {
    void (*old_sigint_handler)(int);
};

static SignalContext global_signal_context = {0};

struct ApplicationContext {
    bool is_running;
    Mutex mutex;
    ConditionVariable cv;
};

static ApplicationContext app_context = {0};

void sigint_handler(int signal_number) {
    if (signal_number == SIGINT) {
        acquire_mutex(&app_context.mutex);
        app_context.is_running = false;
        release_mutex(&app_context.mutex);
        notify_all_condition_variable(&app_context.cv);
    }
    
    if (global_signal_context.old_sigint_handler != 0) (*global_signal_context.old_sigint_handler)(signal_number);
}

void install_signal_handler(int signal_number, void (*handler)(int)) {
    global_signal_context.old_sigint_handler = signal(signal_number, handler);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print("Error, expected 2 arguments, got %d\n", argc);
        return 1;
    }
    
    install_signal_handler(SIGINT, sigint_handler);
    
    app_context.is_running = true;
    app_context.mutex = make_mutex();
    app_context.cv = make_condition_variable();
    
    LspClientContext lsp_client_context = {0};
    if (!initialize(&lsp_client_context, make_const_string(argv[1]))) {
        println("Failed to initialize client context");
        return 1;
    }
    
    acquire_mutex(&app_context.mutex);
    while (app_context.is_running) {
        wait_condition_variable(&app_context.cv, &app_context.mutex);
    }
    release_mutex(&app_context.mutex);
    
    shutdown(&lsp_client_context);
    
    delete_condition_variable(&app_context.cv);
    delete_mutex(&app_context.mutex);
    
    return 0;
}