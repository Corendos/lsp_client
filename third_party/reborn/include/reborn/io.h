/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_IO_H
#define REBORN_IO_H

#include <reborn/types.h>

EXPORT void print(const char* format, ...);
EXPORT void println(const char* format, ...);

EXPORT void eprint(const char* format, ...);
EXPORT void eprintln(const char* format, ...);

EXPORT void set_separator(char comma, char period);

#endif // IO_H
