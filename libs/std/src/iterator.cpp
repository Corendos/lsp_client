#include <std/iterator.h>
#include <std/characters.h>

Iterator make_iterator(const char *data, u64 size) {
    Iterator it = {data, data, size};
    return it;
}

Iterator make_iterator(ConstStringU8 *str) {
    return make_iterator(str->data, str->size);
}

Iterator make_iterator(StringU8 *str) {
    return make_iterator(str->data, str->size);
}

u64 get_position(Iterator *it) {
    return it->current - it->data;
}

bool is_valid(Iterator *it) {
    return (it->current - it->data) < it->size;
}

void skip_to(Iterator* it, char c) {
    if (is_valid(it) && it->current[0] == c) return;
    while (is_valid(it)) {
        it->current++;
        if (it->current[0] == c) return;
    }
}

void skip_to_first_after(Iterator* it, char c) {
    skip_to(it, c);
    it->current++;
}

void skip_to_first_if(Iterator* it, bool (*predicate)(char)) {
    if (is_valid(it) && predicate(it->current[0])) return;
    while (is_valid(it)) {
        it->current++;
        if (predicate(it->current[0])) return;
    }
}

char peek(Iterator* it, u64 offset) {
    return peek_or(it, offset, 0);
}

char peek_or(Iterator* it, u64 offset, char _default) {
    return (it->current + offset - it->data) >= it->size ? _default : it->current[offset];
}

u64 parse_u64(Iterator *it) {
    u64 value = 0;
    while (is_valid(it) && is_numeric(it->current[0])) {
        value = 10 * value + it->current[0] - '0';
        it->current++;
    }
    return value;
}

u64 parse_u64(ConstStringU8 *str) {
    u64 value = 0;
    for (u64 i = 0;i < str->size;++i) {
        char c = str->data[i];
        if (!is_numeric(c)) {
            break;
        }
        value = 10 * value + c - '0';
    }
    return value;
}
