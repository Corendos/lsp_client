/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_ITERATOR_H
#define REBORN_ITERATOR_H

#include <reborn/strings.h>
#include <reborn/types.h>

struct Iterator {
    const char* data;
    const char* current;
    u64 size;
};

EXPORT Iterator make_iterator(const char* data, u64 size);
EXPORT Iterator make_iterator(StringU8* str);
EXPORT Iterator make_iterator(ConstStringU8* str);

EXPORT char peek(Iterator* it, u64 offset);
EXPORT char peek_or(Iterator* it, u64 offset, char _default);

EXPORT u64 get_position(Iterator* it);

EXPORT bool is_valid(Iterator* it);

EXPORT void skip_to(Iterator* it, char c);
EXPORT void skip_to_first_after(Iterator* it, char c);
EXPORT void skip_to_first_if(Iterator* it, bool (*predicate)(char));

EXPORT u64 parse_u64(Iterator* it);
EXPORT u64 parse_u64(ConstStringU8* str);

#endif // REBORN_ITERATOR_H
