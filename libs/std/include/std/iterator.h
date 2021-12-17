#ifndef STD_ITERATOR_H
#define STD_ITERATOR_H

#include <std/types.h>
#include <std/strings.h>

struct Iterator {
    const char *data;
    const char *current;
    u64 size;
};

Iterator make_iterator(const char *data, u64 size);
Iterator make_iterator(ConstStringU8 *str);

char peek(Iterator* it, u64 offset);
char peek_or(Iterator* it, u64 offset, char _default);

u64 get_position(Iterator *it);

bool is_valid(Iterator *it);

void skip_to(Iterator* it, char c);
void skip_to_first_after(Iterator* it, char c);
void skip_to_first_if(Iterator* it, bool (*predicate)(char));

u64 parse_u64(Iterator *it);
u64 parse_u64(ConstStringU8 *str);

#endif // STD_ITERATOR_H
