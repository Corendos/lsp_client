/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_MEMORY_ALLOCATOR_H
#define REBORN_MEMORY_ALLOCATOR_H

#include <reborn/memory/prelude.h>

struct Allocator {
    void* (*allocate_func)(u64 /* size */, u8 /* alignment */, void* /* user_data */);
    void (*free_func)(void* /* data */, void* /* user_data */);
    void* user_data;
};

EXPORT void* _allocate(Allocator* allocator, u64 size, const char* location);
EXPORT void free(Allocator* allocator, void* data);

#define allocate(allocator, size) _allocate((allocator), (size), __FILE_AND_LINE__)

#endif // REBORN_MEMORY_ALLOCATOR_H
