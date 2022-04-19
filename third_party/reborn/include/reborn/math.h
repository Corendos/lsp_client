/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_MATH_H
#define REBORN_MATH_H

#include <reborn/types.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

inl u64 gcd(u64 a, u64 b) {
    while (b != 0) {
        u64 temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

inl i64 gcd(i64 a, i64 b) {
    while (b != 0) {
        i64 temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

#endif // REBORN_MATH_H
