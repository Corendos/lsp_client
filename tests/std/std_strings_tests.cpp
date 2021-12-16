#include <std/strings.h>
#include <std/memory.h>

#include <cstring>
#include <cassert>

test_case(string_creation_test_1) {
    char raw_str[] = "This is a string";
    StringU8 str = make_string(raw_str);
    test_check("The two string should have equal length", strlen(raw_str) == str.size);
    test_check("The two string should be equal", strncmp(raw_str, str.data, str.size) == 0);
    test_check("The created string capacity should be equal to its size", str.size == str.capacity);
}

test_case(string_creation_test_2) {
    char raw_str[] = "This is a string";
    Arena arena = make_arena();
    StringU8 str = push_string(&arena, raw_str);
    test_check("The two string should have equal length", strlen(raw_str) == str.size);
    test_check("The two string should be equal", strncmp(raw_str, str.data, str.size) == 0);
    test_check("The created string capacity should be equal to its size", str.size == str.capacity);
}

test_case(string_creation_test_3) {
    u64 string_capacity = 128;
    Arena arena = make_arena();
    StringU8 str = push_string(&arena, string_capacity);
    test_check("The string should have a size of 0", str.size == 0);
    test_check("The string should have the capacity we asked for", str.capacity == string_capacity);
}

test_case(const_string_creation_test_1) {
    const char raw_str[] = "This is a string";
    ConstStringU8 str = make_const_string(raw_str);
    test_check("The two string should have equal length", strlen(raw_str) == str.size);
    test_check("The two string should be equal", strncmp(raw_str, str.data, str.size) == 0);
}

test_case(const_string_creation_test_2) {
    const char raw_str[] = "This is a string";
    Arena arena = make_arena();
    ConstStringU8 str = push_const_string(&arena, raw_str);
    test_check("The two string should have equal length", strlen(raw_str) == str.size);
    test_check("The two string should be equal", strncmp(raw_str, str.data, str.size) == 0);
}

test_case(string_copy_test) {
    StringU8 str = make_string("This is a string");
    Arena arena = make_arena();
    StringU8 copy = copy_string(&arena, str);
    test_check("The two string should have equal length", str.size == copy.size);
    test_check("The two string should be equal", strncmp(str.data, copy.data, str.size) == 0);
}

test_case(const_string_copy_test) {
    ConstStringU8 str = make_const_string("This is a string");
    Arena arena = make_arena();
    ConstStringU8 copy = copy_const_string(&arena, str);
    test_check("The two string should have equal length", str.size == copy.size);
    test_check("The two string should be equal", strncmp(str.data, copy.data, str.size) == 0);
}

test_case(string_comparison_test) {
    StringU8 str1 = make_string("a");
    StringU8 str2 = make_string("b");
    ConstStringU8 const_str1 = make_const_string("c");
    ConstStringU8 const_str2 = make_const_string("d");
    
    test_check("str1 should be equal to str1", string_compare(str1, str1) == 0);
    test_check("str1 should be less than str2", string_compare(str1, str2) < 0);
    test_check("str2 should be greater than str1", string_compare(str2, str1) > 0);
    test_check("const_str1 should be equal to const_str1", string_compare(const_str1, const_str1) == 0);
    test_check("const_str1 should be less than const_str2", string_compare(const_str1, const_str2) < 0);
    test_check("const_str1 should be greater than const_str2", string_compare(const_str2, const_str1) > 0);
    
    test_check("str1 should be less than const_str1", string_compare(str1, const_str1) < 0);
    test_check("const_str2 should be greater than str2", string_compare(const_str2, str2) > 0);
}

test_case(string_write_to_test_1) {
    Arena arena = make_arena();
    StringU8 str = push_string(&arena, 128);
    test_constraint(str.data != 0);
    
    bool result = write_to(&str, 'E');
    test_check("The string should have a size of 1", str.size == 1);
    test_check("The string should be 'E'", string_compare(str, SCu8("E")) == 0);
    test_check("The write should be successful", result == true);
}

test_case(string_write_to_test_2) {
    Arena arena = make_arena();
    StringU8 str = push_string(&arena, 2);
    test_constraint(str.data != 0);
    
    // NOTE(Corentin): write 2 characters to fill the string
    bool result1 = write_to(&str, 'O');
    bool result2 = write_to(&str, 'K');
    bool result3 = write_to(&str, 'O');
    test_check("The string should have a size of 2", str.size == 2);
    test_check("The string should be 'OK'", string_compare(str, SCu8("OK")) == 0);
    test_check("The first two writes should be successful", result1 && result2);
    test_check("The third write should not be successful", !result3);
}

test_case(string_format_to_test_1) {
    const char start[] = "123456789:";
    const char remaining[] = "smoll";
    u64 total_size = strlen(start) + strlen(remaining);
    
    u64 string_capacity = 16;
    test_constraint(total_size <= string_capacity);
    
    Arena arena = make_arena();
    StringU8 str = push_string(&arena, string_capacity);
    test_constraint(str.data != 0);
    
    i64 result = format_to(&str, "%s%s", start, remaining);
    test_check("The call should be successful", result > 0 && result < string_capacity);
    test_check("The string should be of the right size", str.size == total_size);
    
    char buffer[128] = {0};
    int concat_result = snprintf(buffer, 128, "%s%s", start, remaining);
    test_constraint(concat_result > 0 && concat_result < 128);
    test_check("The string should be equal to the concatenation of its parts", string_compare(make_const_string(buffer), str) == 0);
}

test_case(string_format_to_test_2) {
    const char start[] = "123456789:";
    const char remaining[] = "huge remaining string";
    u64 total_size = strlen(start) + strlen(remaining);
    
    u64 string_capacity = 16;
    test_constraint(total_size > string_capacity);
    
    Arena arena = make_arena();
    StringU8 str = push_string(&arena, string_capacity);
    test_constraint(str.data != 0);
    
    i64 result = format_to(&str, "%s%s", start, remaining);
    test_check("The call should not be successful", result > 0 && result >= string_capacity);
    test_check("The string should still be empty", str.size == 0);
}

test_case(string_reset_test) {
    Arena arena = make_arena();
    StringU8 str = push_string(&arena, 16);
    test_constraint(str.data != 0);
    
    i64 result = format_to(&str, "test", str.data, str.size);
    test_check("The call should be successful", result > 0 && result < 16);
    reset_string(&str);
    test_check("The string should be empty", str.size == 0);
}

test_case(all_std_string) {
    test_run_test(string_creation_test_1);
    test_run_test(string_creation_test_2);
    test_run_test(string_creation_test_3);
    test_run_test(const_string_creation_test_1);
    test_run_test(const_string_creation_test_2);
    test_run_test(string_copy_test);
    test_run_test(const_string_copy_test);
    test_run_test(string_comparison_test);
    test_run_test(string_write_to_test_1);
    test_run_test(string_write_to_test_2);
    test_run_test(string_format_to_test_1);
    test_run_test(string_format_to_test_2);
    test_run_test(string_reset_test);
}