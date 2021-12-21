#include <std/concurrency/mutex.h>

Mutex make_mutex() {
    Mutex m = {0};
    
    m.native_handle = _make_mutex();
    
    return m;
}

void delete_mutex(Mutex* mutex) {
    _delete_mutex(&mutex->native_handle);
}

void acquire_mutex(Mutex* mutex) {
    _acquire_mutex(&mutex->native_handle);
}

bool try_acquire_mutex(Mutex* mutex) {
    return _try_acquire_mutex(&mutex->native_handle);
}

void release_mutex(Mutex* mutex) {
    _release_mutex(&mutex->native_handle);
}
