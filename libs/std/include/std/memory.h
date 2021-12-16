#ifndef STD_MEMORY_H
#define STD_MEMORY_H

#include <std/types.h>

#define KB(x) (u64)(x) * 1024ull
#define MB(x) (u64)(x) * 1024ull * 1024ull
#define GB(x) (u64)(x) * 1024ull * 1024ull * 1024ull

#define array_size(a) sizeof(a) / sizeof((a)[0])

struct ArenaNode {
    u8 *data;
    u64 size;
    u64 capacity;
    
    ArenaNode *next;
    ArenaNode *prev;
};

struct Arena {
    ArenaNode *first;
    ArenaNode *last;
    
    u64 node_capacity;
};

Arena make_arena(u64 node_capacity = KB(64));
void free_arena(Arena *arena);

void reset_arena(Arena *arena);

void *allocate(Arena *arena, u64 size_in_bytes);
void *allocate(Arena *arena, u64 size, u64 count);

inline u64 align_to(u64 value, u8 alignment_power) {
    if (value != 0) {
        u64 power = 1ull << (u64)alignment_power;
        u64 mask = ~(power - 1);
        return ((value - 1) & mask) + power;
    }
    return 0;
}

inline bool is_aligned_to(u64 value, u8 alignment_power) {
    u64 power = 1ull << (u64)alignment_power;
    return value % power == 0;
}

struct Buffer {
    u8* data;
    u64 size;
    u64 capacity;
};

Buffer push_buffer(Arena* arena, u64 size);
void reset_buffer();

#endif // STD_MEMORY_H
