#ifndef STD_STRINGS_H
#define STD_STRINGS_H

#include <std/types.h>
#include <std/memory.h>

#include <cstdarg>

#define expand_string(str) (int)(str.size), (str.data)

struct ConstStringU8 {
    const char *data;
    u64 size;
};

struct ConstStringU8Array {
    ConstStringU8* values;
    u64 count;
};

struct StringU8 {
    char *data;
    u64 size;
    u64 capacity;
};

struct StringU8Array {
    StringU8* strings;
    u64 count;
};

StringU8 make_string(char *str, u64 size);
StringU8 make_string(char *str);

void reset_string(StringU8 *str);

ConstStringU8 make_const_string(const char *str, u64 size);
ConstStringU8 make_const_string(const char *str);
ConstStringU8 make_const_string(StringU8 source);

int string_compare(ConstStringU8 a, ConstStringU8 b);
int string_compare(StringU8 a, StringU8 b);
int string_compare(ConstStringU8 a, StringU8 b);
int string_compare(StringU8 a, ConstStringU8 b);

StringU8 push_string(Arena* arena, const char *str, u64 size);
StringU8 push_string(Arena* arena, const char *str);
StringU8 push_string(Arena* arena, u64 size);
StringU8 copy_string(Arena* arena, StringU8 other);

ConstStringU8 push_const_string(Arena* arena, const char *str, u64 size);
ConstStringU8 push_const_string(Arena* arena, const char *str);
ConstStringU8 copy_const_string(Arena* arena, ConstStringU8 other);

#define SCu8(raw_str) ConstStringU8{(raw_str), (sizeof(raw_str) / sizeof(*(raw_str))) - 1}

bool write_to(StringU8 *str, char c);
i64 format_to(StringU8 *str, const char* format, ...);
i64 format_to_v(StringU8 *str, const char* format, va_list args);

#endif // STD_STRINGS_H
