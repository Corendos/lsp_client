/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_CONCURRENCY_MUTEX_WIN32_H
#define REBORN_CONCURRENCY_MUTEX_WIN32_H

#include <windows.h>

struct NativeMutex {
    CRITICAL_SECTION cs;
};

#endif // REBORN_CONCURRENCY_MUTEX_WIN32_H
