#include <std/iterator.h>
#include <std/strings.h>

static const char ITERATOR_INPUT[] = 
"...........\n"
"1..........\n"
"2..........\n"
"...........\n"
"......642;.\n"
"...........\n"
"...........\n"
"...........\n";

test_case(test_make_iterator_1) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    test_check("The iterator has the correct data ptr", it.data == ITERATOR_INPUT);
    test_check("The iterator current pointer points to the beginning of the input", it.current == ITERATOR_INPUT);
    test_check("The iterator has the correct input size", it.size == array_size(ITERATOR_INPUT));
}

test_case(test_make_iterator_2) {
    ConstStringU8 str = make_const_string(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    Iterator it = make_iterator(&str);
    test_check("The iterator has the correct data ptr", it.data == ITERATOR_INPUT);
    test_check("The iterator current pointer points to the beginning of the input", it.current == ITERATOR_INPUT);
    test_check("The iterator has the correct input size", it.size == array_size(ITERATOR_INPUT));
}

test_case(test_peek) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    u64 offset = 12;
    const char *c = ITERATOR_INPUT + offset;
    test_constraint(*c == '1');
    test_check("Calling peek with an offset of 12 should return the first 1", peek(&it, offset) == '1');
    u64 big_offset = 2000;
    test_constraint(it.size < big_offset);
    test_check("Calling peek with an offset of 2000 should return 0", peek(&it, big_offset) == 0);
}

test_case(test_peek_or) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    u64 big_offset = 2000;
    test_constraint(it.size < big_offset);
    test_check("Calling peek_or with an offset of 2000 and 'a' should return 'a'", peek_or(&it, big_offset, 'a') == 'a');
}

test_case(test_get_position) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    test_check("An iterator freshly created should have a position of 0", get_position(&it) == 0);
    u64 advance = 12;
    test_constraint(it.size > advance);
    it.current += advance;
    test_check("An iterator that advanced by 12 should have a position of 12", get_position(&it) == advance);
}

test_case(test_is_valid) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    test_check("An iterator freshly created should be valid if the data has a size > 1", is_valid(&it));
    u64 advance = 12;
    test_constraint(it.size > advance);
    it.current += advance;
    test_check("An iterator that advanced by a number less than the input size should be valid", is_valid(&it));
    u64 big_advance = 2000;
    test_constraint(it.size < big_advance);
    it.current += big_advance;
    test_check("An iterator that advanced by a number greater than the input size should not be valid", !is_valid(&it));
}

test_case(test_skip_to) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    skip_to(&it, '1');
    test_check("The iterator should have skipped to the first 1 it encountered", it.current[0] == '1');
    test_check("The iterator should have the correct position", get_position(&it) == 12);
}

test_case(test_skip_to_first_after) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    skip_to_first_after(&it, '\n');
    test_check("The iterator should have skipped to the character after the first '\\n'", it.current[0] == '1');
    test_check("The iterator should have the correct position", get_position(&it) == 12);
}

bool is_two(char c) { return c == '2'; }

test_case(test_skip_to_first_if) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    skip_to_first_if(&it, is_two);
    test_check("The iterator should have skipped to the character validating the predicate", it.current[0] == '2');
    test_check("The iterator has the correct position", get_position(&it) == 24);
}

test_case(test_parse_u64_1) {
    Iterator it = make_iterator(ITERATOR_INPUT, array_size(ITERATOR_INPUT));
    skip_to(&it, '6');
    test_constraint(it.current[0] == '6');
    u64 value = parse_u64(&it);
    test_check("The parsed value should be correct", value == 642);
    test_check("The iterator should point to the first character after the number in the input", it.current[0] == ';');
}

test_case(all_std_iterator) {
    test_run_test(test_make_iterator_1);
    test_run_test(test_make_iterator_2);
    test_run_test(test_peek);
    test_run_test(test_peek_or);
    test_run_test(test_get_position);
    test_run_test(test_is_valid);
    test_run_test(test_skip_to);
    test_run_test(test_skip_to_first_after);
    test_run_test(test_skip_to_first_if);
    test_run_test(test_parse_u64_1);
}