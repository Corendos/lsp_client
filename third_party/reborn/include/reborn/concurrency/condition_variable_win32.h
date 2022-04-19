/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_CONCURRENCY_CONDITION_VARIABLE_WIN32_H
#define REBORN_CONCURRENCY_CONDITION_VARIABLE_WIN32_H

#include <windows.h>

struct NativeConditionVariable {
    CONDITION_VARIABLE cv;
};

#endif //REBORN_CONCURRENCY_CONDITION_VARIABLE_WIN32_H
