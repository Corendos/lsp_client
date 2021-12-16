#include <std/strings.h>
#include <std/memory.h>

#include <cstring>
#include <cstdio>

StringU8 make_string(char *str, u64 size) {
    return StringU8{str, size, size};
}

StringU8 make_string(char *str) {
    return make_string(str, (u64)strlen(str));
}

void reset_string(StringU8 *str) {
    str->size = 0;
}

ConstStringU8 make_const_string(const char *str, u64 size) {
    return ConstStringU8{str, size};
}

ConstStringU8 make_const_string(const char *str) {
    return make_const_string(str, (u64)strlen(str));
}

ConstStringU8 make_const_string(StringU8 source) {
    return make_const_string(source.data, source.size);
}

int string_compare(ConstStringU8 a, ConstStringU8 b) {
    return strncmp(a.data, b.data, a.size < b.size ? a.size : b.size);
}

int string_compare(StringU8 a, StringU8 b) {
    return string_compare(make_const_string(a), make_const_string(b));
}

int string_compare(ConstStringU8 a, StringU8 b) {
    return string_compare(a, make_const_string(b));
}

int string_compare(StringU8 a, ConstStringU8 b) {
    return string_compare(make_const_string(a), b);
}

StringU8 push_string(Arena* arena, const char *str, u64 size) {
    StringU8 string = {0};
    string.size = size;
    string.capacity = size;
    string.data = (char*)allocate(arena, size + 1);
    memcpy(string.data, str, size);
    string.data[size] = '\0';
    return string;
}

StringU8 push_string(Arena* arena, const char *str) {
    return push_string(arena, str, strlen(str));
}

StringU8 push_string(Arena* arena, u64 size) {
    StringU8 string = {0};
    string.size = 0;
    string.capacity = size;
    string.data = (char*)allocate(arena, size + 1);
    return string; 
}

StringU8 copy_string(Arena* arena, StringU8 other) {
    return push_string(arena, other.data, other.size);
}

ConstStringU8 push_const_string(Arena* arena, const char *str, u64 size) {
    ConstStringU8 string = {0};
    string.size = size;
    char *data = (char*)allocate(arena, size + 1);
    memcpy(data, str, size);
    data[size] = '\0';
    string.data = data;
    return string;
}

ConstStringU8 push_const_string(Arena* arena, const char *str) {
    return push_const_string(arena, str, strlen(str));
}

ConstStringU8 copy_const_string(Arena* arena, ConstStringU8 other) {
    return push_const_string(arena, other.data, other.size);
}

bool write_to(StringU8 *str, char c) {
    if (str->size == str->capacity) return false;
    str->data[str->size++] = c;
    return true;
}

i64 format_to(StringU8 *str, const char* format, ...) {
    va_list args;
    va_start(args, format);
    i64 result = format_to_v(str, format, args);
    va_end(args);
    return result;
}

i64 format_to_v(StringU8 *str, const char* format, va_list args) {
    // NOTE(Corentin): the string is allocated to have an extra '\0' at the end
    u64 remaining = str->capacity - str->size + 1;
    i64 result = (i64)vsnprintf(str->data + str->size, remaining, format, args);
    if (result < remaining) {
        str->size += result;
        return result;
    }
    // NOTE(Corentin): we discard the characters that have potentially been written a;ready
    str->data[str->size] = 0;
    return result;
}