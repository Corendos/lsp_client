#ifndef REBORN_TIME_TYPES_H
#define REBORN_TIME_TYPES_H

#include <reborn/types.h>

struct Rational {
    i64 num;
    i64 den;
};

struct FloatingTimePoint {
    f64 value;
    Rational ratio;
};

struct FloatingDuration {
    f64 value;
    Rational ratio;
};

struct TimePoint {
    i64 value;
    Rational ratio;
};

struct Duration {
    i64 value;
    Rational ratio;
};

#endif // REBORN_TIME_TYPES_H
