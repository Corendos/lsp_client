/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_MEMORY_BUFFER_H
#define REBORN_MEMORY_BUFFER_H

#include <reborn/memory/allocator.h>
#include <reborn/memory/prelude.h>

struct ConstBuffer {
    const u8* data;
    u64 size;
};

struct Buffer {
    u8* data;
    u64 size;
    u64 capacity;
};

EXPORT ConstBuffer make_const_buffer(const u8* data, u64 size);
EXPORT ConstBuffer make_const_buffer(Buffer buffer);
EXPORT ConstBuffer copy_const_buffer(Allocator* allocator, ConstBuffer source);
EXPORT Buffer copy_buffer(Allocator* allocator, ConstBuffer source);
EXPORT bool copy_buffer(Buffer* dest, ConstBuffer source);
EXPORT Buffer push_buffer(Allocator* allocator, u64 size);
EXPORT void reset_buffer(Buffer* buffer);

// NOTE(Corentin): Temporary
void print_buffer_hex(ConstBuffer buffer, Arena* temporary_arena);

#endif // REBORN_MEMORY_BUFFER_H
