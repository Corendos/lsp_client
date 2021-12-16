#ifndef TESTS_H
#define TESTS_H

#include <cstdlib>
#include <cstdio>
#include <cstring>

#define _STRINGIFY(s) STRINGIFY(s)
#define STRINGIFY(s) #s
#define CONCAT(a, b) a ## b

#define GREEN_OUTPUT "\x1b\x5b" "32m"
#define RED_OUTPUT "\x1b\x5b" "31m"
#define DEFAULT_OUTPUT "\x1b\x5b" "0m"

#define GREEN_FG(text) "\x1b\x5b" "32m" text "\x1b\x5b" "0m"
#define RED_FG(text) "\x1b\x5b" "31m" text "\x1b\x5b" "0m"

static const int INDENT_SIZE = 2;

struct TestContext {
    int test_run_count;
    int test_success_count;
    int test_failure_count;
    int indent_level;
};

static TestContext context = {0};

static const char success_str[] = "\x1b\x5b" "32m" "S" "\x1b\x5b" "0m";
static const char failure_str[] = "\x1b\x5b" "31m" "F" "\x1b\x5b" "0m";

#define test_check(message, test)\
do {\
bool success = test;\
char temp_buffer_format[79] = {0};\
memset(temp_buffer_format, '.', 78);\
int message_length = strlen(message);\
int space_needed = 79 - context.indent_level - message_length;\
bool need_ellipsis = false;\
if (space_needed < 1) {\
space_needed = 1;\
message_length = 79 - context.indent_level - space_needed;\
message_length = message_length < 0 ? 0 : message_length;\
need_ellipsis = true;\
}\
strncpy(temp_buffer_format, message, message_length);\
if (need_ellipsis) {\
temp_buffer_format[message_length - 1] = '.';\
temp_buffer_format[message_length - 2] = '.';\
temp_buffer_format[message_length - 3] = '.';\
}\
printf("%*s%s %s\n", context.indent_level, "", temp_buffer_format, success ? success_str : failure_str);\
_test_framework_result = _test_framework_result && success;\
} while (0)

#define test_require(message, test)\
do {\
bool success = test;\
char temp_buffer_format[79] = {0};\
memset(temp_buffer_format, '.', 78);\
int message_length = strlen(message);\
int space_needed = 79 - context.indent_level - message_length;\
bool need_ellipsis = false;\
if (space_needed < 1) {\
space_needed = 1;\
message_length = 79 - context.indent_level - space_needed;\
message_length = message_length < 0 ? 0 : message_length;\
need_ellipsis = true;\
}\
strncpy(temp_buffer_format, message, message_length);\
if (need_ellipsis) {\
temp_buffer_format[message_length - 1] = '.';\
temp_buffer_format[message_length - 2] = '.';\
temp_buffer_format[message_length - 3] = '.';\
}\
printf("%*s%s %s\n", context.indent_level, "", temp_buffer_format, success ? success_str : failure_str);\
_test_framework_result = _test_framework_result && success;\
if (!success) return;\
} while (0)

#define test_constraint(test)\
do {\
bool success = test;\
if (!success) {\
printf("\x1b\x5b" "31m" "!!!" "\x1b\x5b" "0m"" Error, test constraint '%s' is false, aborting this test\n", STRINGIFY(test));\
_test_framework_result = false;\
return;\
}\
} while (0)

#define test_case(test_name) \
void CONCAT(test_name, _proxy)(bool &_test_framework_result); \
bool test_name() {\
bool _test_framework_result = true;\
CONCAT(test_name, _proxy)(_test_framework_result);\
return _test_framework_result;\
}\
void CONCAT(test_name, _proxy)(bool &_test_framework_result)

#define test_run_test(test)                                               \
do {                                                                  \
const char test_name[] = STRINGIFY(test);                         \
printf("%*sTest: \"%s\"\n", context.indent_level, "", test_name); \
context.indent_level += INDENT_SIZE;                              \
bool success = test();                                            \
context.indent_level -= INDENT_SIZE;                              \
context.test_run_count++;                                         \
if (success) {                                                    \
context.test_success_count++;                                 \
} else {                                                          \
context.test_failure_count++;                                 \
}                                                                 \
_test_framework_result= _test_framework_result && success;        \
} while (0)

#endif //TESTS_H
