#include <std/memory.h>

test_case(align_to_test_1) {
    u64 alignment_power = 3;
    u64 power = 1ull << alignment_power;
    u64 aligned = align_to(0, alignment_power);
    test_check("0 should be aligned to 0", aligned == 0);
}

test_case(align_to_test_2) {
    u64 alignment_power = 3;
    u64 power = 1ull << alignment_power;
    u64 aligned = align_to(3, alignment_power);
    test_check("3 should be aligned to 8", aligned == 8);
}

test_case(align_to_test_3) {
    u64 alignment_power = 3;
    u64 power = 1ull << alignment_power;
    u64 aligned = align_to(8, alignment_power);
    test_check("8 should be aligned to 8", aligned == 8);
}

test_case(aligned_to_test) {
    test_check("0 is aligned to 8", is_aligned_to(0, 3));
    test_check("8 is aligned to 8", is_aligned_to(8, 3));
    test_check("3 is not aligned to 8", !is_aligned_to(3, 3));
}

test_case(make_arena_tests) {
    Arena arena = make_arena();
    test_check("The default arena node capacity is 64kB", arena.node_capacity == KB(64));
    test_check("The arena is empty on creation", arena.first == 0 && arena.last == 0);
}

test_case(make_arena_custom_size_tests) {
    u64 capacity = MB(2);
    Arena arena = make_arena(capacity);
    test_check("The default arena node capacity is the one given", arena.node_capacity == capacity);
    test_check("The arena is empty on creation", arena.first == 0 && arena.last == 0);
}

test_case(arena_usage_tests) {
    Arena arena = make_arena();
    void *ptr1 = allocate(&arena, 3);
    void *ptr2 = allocate(&arena, 42);
    test_constraint(ptr1 != 0);
    test_constraint(ptr2 != 0);
    test_check("The pointers should be aligned on a 8-byte boundary", is_aligned_to((u64)ptr1, 3) && is_aligned_to((u64)ptr2, 3));
    void *big_ptr = allocate(&arena, KB(128));
    test_constraint(big_ptr != 0);
    test_check("The pointers should be aligned on a 8-byte boundary", is_aligned_to((u64)big_ptr, 3));
}

test_case(all_std_memory) {
    test_run_test(align_to_test_1);
    test_run_test(align_to_test_2);
    test_run_test(align_to_test_3);
    test_run_test(aligned_to_test);
    test_run_test(make_arena_tests);
    test_run_test(make_arena_custom_size_tests);
    test_run_test(arena_usage_tests);
}