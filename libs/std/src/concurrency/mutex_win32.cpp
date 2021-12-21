#include <std/concurrency/mutex_win32.h>

NativeMutex _make_mutex() {
    NativeMutex m = {0};
    
    InitializeCriticalSection(&m.cs);
    
    return m;
}

void _delete_mutex(NativeMutex* mutex) {
    DeleteCriticalSection(&mutex->cs);
}

void _acquire_mutex(NativeMutex* mutex) {
    EnterCriticalSection(&mutex->cs);
}

bool _try_acquire_mutex(NativeMutex* mutex) {
    return TryEnterCriticalSection(&mutex->cs) != 0;
}

void _release_mutex(NativeMutex* mutex) {
    LeaveCriticalSection(&mutex->cs);
}
