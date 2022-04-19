/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_CONCURRENCY_THREAD_H
#define REBORN_CONCURRENCY_THREAD_H

#include <reborn/strings.h>

#if defined(OS_WINDOWS)
#include "thread_win32.h"
#else
#error "No implementation for type Thread"
#endif

struct Thread {
    StringU8 name;
    NativeThread native_handle;
};

EXPORT Thread make_thread(ConstStringU8 name, u64 (*thread_fun)(void*), void* parameters);
EXPORT void join_thread(Thread* thread);
EXPORT bool is_thread_joinable(Thread* thread);
EXPORT void detach_thread(Thread* thread);

#endif // REBORN_CONCURRENCY_THREAD_H
