#include <std/memory.h>
#include <std/math.h>

#include <cstdlib>
#include <cassert>

Arena make_arena(u64 node_capacity) {
    Arena arena = {0};
    
    arena.node_capacity = node_capacity;
    
    return arena;
}

void free_arena(Arena *arena) {
    ArenaNode *current = arena->first;
    
    while (current) {
        ArenaNode *next = current->next;
        free(current);
        current = next;
    }
}

void reset_arena(Arena *arena) {
    ArenaNode *current = arena->first;
    
    while (current) {
        current->size = 0;
        current = current->next;
    }
    
    arena->last = arena->first;
}

ArenaNode *arena_allocate_node(Arena* arena, u64 node_capacity) {
    u64 header_aligned_size = align_to(sizeof(ArenaNode), 3);
    u8* storage = (u8*)calloc(1, node_capacity + header_aligned_size);
    ArenaNode* new_node = (ArenaNode*)storage;
    
    new_node->capacity = node_capacity;
    new_node->prev = arena->last;
    if (arena->last) {
        arena->last->next = new_node;
    }
    arena->last = new_node;
    
    new_node->data = storage + header_aligned_size;
    
    return new_node;
}

void *allocate(Arena *arena, u64 size_in_bytes) {
    assert(size_in_bytes != 0);
    ArenaNode *last_node = arena->last;
    
    if (!last_node) {
        last_node = arena_allocate_node(arena, arena->node_capacity);
        arena->first = last_node;
    }
    
    u64 aligned_size = align_to(size_in_bytes, 3);
    
    if (last_node->capacity - last_node->size < aligned_size) {
        u64 new_node_capacity = max(size_in_bytes, arena->node_capacity);
        last_node = arena_allocate_node(arena, new_node_capacity);
        aligned_size = new_node_capacity;
    }
    
    void *ptr = last_node->data + last_node->size;
    last_node->size += aligned_size;
    return ptr;
}

void *allocate(Arena *arena, u64 size, u64 count) {
    return allocate(arena, size * count);
}

Buffer push_buffer(Arena* arena, u64 size) {
    Buffer buffer = {0, 0, size};
    buffer.data = (u8*)allocate(arena, size);
    return buffer;
}

void reset_buffer(Buffer* buffer) {
    buffer->size = 0;
}