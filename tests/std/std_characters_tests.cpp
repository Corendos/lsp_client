#include <std/characters.h>

test_case(is_alpha_test) {
    bool result = true;
    for (char c = 'a';c <= 'z';++c) {
        result &= is_alpha(c);
    }
    test_check("characters 'a' to 'z' should be considered as alpha", result);
    
    result = true;
    for (char c = 'A';c <= 'Z';++c) {
        result &= is_alpha(c);
    }
    test_check("characters 'A' to 'Z' should be considered as alpha", result);
    
    result = true;
    for (char c = '0';c <= '9';++c) {
        result &= !is_alpha(c);
    }
    test_check("characters '0' to '9' should not be considered as alpha", result);
}

test_case(is_numeric_test) {
    bool result = true;
    for (char c = 'a';c <= 'z';++c) {
        result &= !is_numeric(c);
    }
    test_check("characters 'a' to 'z' should not be considered as numeric", result);
    
    result = true;
    for (char c = 'A';c <= 'Z';++c) {
        result &= !is_numeric(c);
    }
    test_check("characters 'A' to 'Z' should not be considered as numeric", result);
    
    result = true;
    for (char c = '0';c <= '9';++c) {
        result &= is_numeric(c);
    }
    test_check("characters '0' to '9' should be considered as numeric", result);
}

test_case(is_alphanumeric_test) {
    bool result = true;
    for (char c = 'a';c <= 'z';++c) {
        result &= is_alphanumeric(c);
    }
    test_check("characters 'a' to 'z' should be considered as alpha", result);
    
    result = true;
    for (char c = 'A';c <= 'Z';++c) {
        result &= is_alphanumeric(c);
    }
    test_check("characters 'A' to 'Z' should be considered as alpha", result);
    
    result = true;
    for (char c = '0';c <= '9';++c) {
        result &= is_alphanumeric(c);
    }
    test_check("characters '0' to '9' should be considered as alpha", result);
}

test_case(is_hexadecimal_test) {
    bool result = true;
    for (char c = 'a';c <= 'f';++c) {
        result &= is_alphanumeric(c);
    }
    test_check("characters 'a' to 'f' should be considered as hexadecimal", result);
    
    result = true;
    for (char c = 'A';c <= 'F';++c) {
        result &= is_alphanumeric(c);
    }
    test_check("characters 'A' to 'F' should be considered as hexadecimal", result);
    
    result = true;
    for (char c = '0';c <= '9';++c) {
        result &= is_alphanumeric(c);
    }
    test_check("characters '0' to '9' should be considered as hexadecimal", result);
}

test_case(is_space_test) {
    test_check("' ' should be considered as a space", is_space(' '));
    test_check("'\\t' should be considered as a space", is_space('\t'));
    test_check("'\\n' should be considered as a space", is_space('\n'));
    test_check("'\\v' should be considered as a space", is_space('\v'));
    test_check("'\\f' should be considered as a space", is_space('\f'));
    test_check("'\\r' should be considered as a space", is_space('\r'));
}

test_case(all_std_characters) {
    test_run_test(is_alpha_test);
    test_run_test(is_numeric_test);
    test_run_test(is_alphanumeric_test);
    test_run_test(is_hexadecimal_test);
    test_run_test(is_space_test);
}