/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_MEMORY_PRELUDE_H
#define REBORN_MEMORY_PRELUDE_H

#include <reborn/types.h>

#define KB(x) (u64)(x) * 1024ull
#define MB(x) (u64)(x) * 1024ull * 1024ull
#define GB(x) (u64)(x) * 1024ull * 1024ull * 1024ull

#define array_size(a) sizeof(a) / sizeof(*(a))

#if ENABLE_CUSTOM_MEMORY_FUNCTION

#include <immintrin.h>
#include <intrin.h>

inl void memory_copy(const void* src, void* dest, u64 count) {
    const u8* src_byte = (const u8*)src;
    u8* dest_byte = (u8*)dest;
    if (src_byte > dest_byte) {
        for (u64 i = 0; i < count; ++i) {
            *(dest_byte++) = *(src_byte++);
        }
    } else {
        for (u64 i = 0; i < count; ++i) {
            *(dest_byte + count - i - 1) = *(src_byte + count - i - 1);
        }
    }
}

inl i32 memory_compare(const void* a, const void* b, u64 count) {
    const u8* a_byte = (const u8*)a;
    const u8* b_byte = (const u8*)b;

    for (u64 i = 0; i < count; ++i) {
        i16 value = (i16) * (a_byte++) - (i16) * (b_byte++);
        if (value != 0) return value;
    }

    return 0;
}

inl void memory_set(void* dest, u8 value, u64 count) {
    u8* dest_byte = (u8*)dest;
    while (count--) {
        /*}
        for (u64 i = 0; i < count; ++i) {*/
        *(dest_byte++) = value;
    }
}

inl void _fallback_memory_set(void* dest, u8 value, u64 count) {
    u8* dest_byte = (u8*)dest;
    while (count--) {
        *(dest_byte++) = value;
    }
}

inl void _simd_memory_set(void* dest_, u8 value, u64 count) {
    u8* dest = (u8*)dest_;

    // Store first 32 bytes unaligned
    __m256i a = _mm256_set1_epi8(value);
    _mm256_storeu_si256((__m256i*)dest, a);

    // Find the next aligned address
    u8* addr = (u8*)((u64)dest & ~0b11111) + 32;
    u64 remaining = count - (addr - dest);

    // Use 4 aligned store as much as possible
    while (remaining > 128) {
        _mm256_store_si256((__m256i*)addr, a);
        _mm256_store_si256((__m256i*)(addr + 32), a);
        _mm256_store_si256((__m256i*)(addr + 64), a);
        _mm256_store_si256((__m256i*)(addr + 96), a);
        addr += 128;
        remaining -= 128;
    }

    // Store the last 3 aligned
    u8* end_addr = (u8*)((u64)(dest + count - 96) & ~0b11111);
    _mm256_store_si256((__m256i*)end_addr, a);
    _mm256_store_si256((__m256i*)(end_addr + 32), a);
    _mm256_store_si256((__m256i*)(end_addr + 64), a);

    // Store the last aligned
    _mm256_storeu_si256((__m256i*)(dest + count - 32), a);
    _mm_sfence();
}

inl void _optimized_memory_set(void* dest, u8 value, u64 count) {
    if (count < 1204) {
        _simd_memory_set(dest, value, count);
    } else {
        __stosb((u8*)dest, value, count);
    }
}

#else
#include <string.h>
inl void memory_copy(const void* src, void* dest, u64 count) { memmove(dest, src, count); }

inl i32 memory_compare(const void* a, const void* b, u64 count) { return memcmp(a, b, count); }

inl void memory_set(void* dest, u8 value, u64 count) { memset(dest, value, count); }

#endif
#endif // REBORN_MEMORY_PRELUDE_H
