/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_CHARACTERS_H
#define REBORN_CHARACTERS_H

#include <reborn/types.h>

inl bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

inl bool is_numeric(char c) { return c >= '0' && c <= '9'; }

inl bool is_alphanumeric(char c) { return is_alpha(c) || is_numeric(c); }

inl bool is_hexadecimal(char c) { return is_numeric(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

inl bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

#endif // REBORN_CHARACTERS_H
