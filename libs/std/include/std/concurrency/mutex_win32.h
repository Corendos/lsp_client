#ifndef STD_CONCURRENCY_MUTEX_WIN32_H
#define STD_CONCURRENCY_MUTEX_WIN32_H

#include <windows.h>

struct NativeMutex {
    CRITICAL_SECTION cs;
};

NativeMutex _make_mutex();
void _delete_mutex(NativeMutex* mutex);
void _acquire_mutex(NativeMutex* mutex);
bool _try_acquire_mutex(NativeMutex* mutex);
void _release_mutex(NativeMutex* mutex);

#endif // STD_CONCURRENCY_MUTEX_WIN32_H
