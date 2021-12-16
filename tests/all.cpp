#include "tests.h"

#include <cstdio>

#include "std/all.cpp"

test_case(all) {
    test_run_test(all_std);
}

int main(int argc, char **argv) {
    all();
    printf("Success: " GREEN_FG("%d") " - Failure: " RED_FG("%d") " - Total: %d\n", context.test_success_count, context.test_failure_count, context.test_run_count);
    if (context.test_failure_count > 0) {
        printf(RED_FG("Failure")"\n");
        //printf("\x1b\x5b" "31m" "FAILURE" "\x1b\x5b" "0m\n");
    } else {
        printf(GREEN_FG("SUCCESS !")"\n");
    }
    return 0;
}