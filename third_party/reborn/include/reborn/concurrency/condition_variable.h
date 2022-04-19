/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_CONCURRENCY_CONDITION_VARIABLE_H
#define REBORN_CONCURRENCY_CONDITION_VARIABLE_H

#include <reborn/types.h>
#include <reborn/concurrency/mutex.h>

#if defined(OS_WINDOWS)
#include "condition_variable_win32.h"
#else
#error "No implementation for type ConditionVariable"
#endif

struct ConditionVariable {
    NativeConditionVariable native_handle;
};

EXPORT ConditionVariable make_condition_variable();
EXPORT void delete_condition_variable(ConditionVariable* condition_variable);
EXPORT void notify_one_condition_variable(ConditionVariable* condition_variable);
EXPORT void notify_all_condition_variable(ConditionVariable* condition_variable);
EXPORT void wait_condition_variable(ConditionVariable* condition_variable, Mutex* mutex);

#endif //REBORN_CONCURRENCY_CONDITION_VARIABLE_H
