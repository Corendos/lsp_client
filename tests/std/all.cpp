#include "std_memory_tests.cpp"
#include "std_strings_tests.cpp"
#include "std_characters_tests.cpp"

test_case(all_std) {
    test_run_test(all_std_memory);
    test_run_test(all_std_string);
    test_run_test(all_std_characters);
}