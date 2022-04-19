#ifndef REBORN_TIME_CLOCK_H
#define REBORN_TIME_CLOCK_H

#include <reborn/time/types.h>

EXPORT FloatingTimePoint convert_with_ratio(FloatingTimePoint t, Rational r);
EXPORT FloatingTimePoint convert_floating_to_ns(FloatingTimePoint t);
EXPORT FloatingTimePoint convert_floating_to_us(FloatingTimePoint t);
EXPORT FloatingTimePoint convert_floating_to_ms(FloatingTimePoint t);
EXPORT FloatingTimePoint convert_floating_to_s(FloatingTimePoint t);

EXPORT FloatingTimePoint get_floating_time_ns();
EXPORT FloatingTimePoint get_floating_time_us();
EXPORT FloatingTimePoint get_floating_time_ms();
EXPORT FloatingTimePoint get_floating_time_s();

EXPORT TimePoint convert_with_ratio(TimePoint t, Rational r);
EXPORT TimePoint convert_to_ns(TimePoint t);
EXPORT TimePoint convert_to_us(TimePoint t);
EXPORT TimePoint convert_to_ms(TimePoint t);
EXPORT TimePoint convert_to_s(TimePoint t);

EXPORT TimePoint get_time_ns();
EXPORT TimePoint get_time_us();
EXPORT TimePoint get_time_ms();
EXPORT TimePoint get_time_s();

#endif // REBORN_TIME_CLOCK_H
