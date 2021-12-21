#ifndef STD_CONCURRENCY_MUTEX_H
#define STD_CONCURRENCY_MUTEX_H

#if defined(PLATFORM_WIN32)
#include "mutex_win32.h"
#else
#error "No implementation for type Mutex"
#endif

struct Mutex {
    NativeMutex native_handle;
};

Mutex make_mutex();
void delete_mutex(Mutex* mutex);
void acquire_mutex(Mutex* mutex);
bool try_acquire_mutex(Mutex* mutex);
void release_mutex(Mutex* mutex);

#endif // STD_CONCURRENCY_MUTEX_H
