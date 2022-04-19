/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_CONCURRENCY_THREAD_WIN32_H
#define REBORN_CONCURRENCY_THREAD_WIN32_H

#include <reborn/strings.h>

#include <windows.h>

struct NativeThread {
    HANDLE handle;
    DWORD id;
};

NativeThread _make_thread(ConstStringU8 name, u64 (*thread_fun)(void*), void* parameters);
void _join_thread(NativeThread* thread);
bool _is_thread_joinable(NativeThread* thread);
void _detach_thread(NativeThread* thread);

#endif // REBORN_CONCURRENCY_THREAD_WIN32_H
