#ifndef TEST_H
#define TEST_H

typedef struct test_suite {
    int total;
    int passed;
} test_suite;

void test_condition(test_suite *suite, const char *name, int condition);
void test_summary(const test_suite *suite);

#endif // TEST_H

#ifdef TEST_IMPLEMENTATION

#include <stdio.h>

#define TEST_COLOR_RED "\x1b[31m"
#define TEST_COLOR_GREEN "\x1b[32m"
#define TEST_COLOR_RESET "\x1b[0m"

void test_condition(test_suite *suite, const char *name, int condition) {
    suite->total++;
    if (condition) {
        suite->passed++;
        printf("[PASS] %s\n" , name);
    } else
        printf(TEST_COLOR_RED "[FAIL] %s\n" TEST_COLOR_RESET, name);
}

void test_summary(const test_suite *suite) {
    printf("==================================\n");
    if (suite->passed == suite->total)
        printf(TEST_COLOR_GREEN "All tests are passing!\n" TEST_COLOR_RESET);
    printf("Summary\n");
    printf("Total: %d, Passed: %d, Failed: %d\n", suite->total, suite->passed, suite->total-suite->passed);
}

#endif // TEST_IMPLEMENTATION
