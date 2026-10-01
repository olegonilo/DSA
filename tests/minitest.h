/* minitest.h — мінімальний тест-фреймворк: CHECK не зупиняє тест, TEST_MAIN друкує підсумок. */
#ifndef MINITEST_H
#define MINITEST_H

#include <stdio.h>
#include <stdlib.h>

static int mt_failed, mt_checks;

#define CHECK(cond) do { \
    mt_checks++; \
    if (!(cond)) { mt_failed++; fprintf(stderr, "  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } \
} while (0)

#define CHECK_EQ_INT(a, b) do { \
    long long _a = (long long)(a), _b = (long long)(b); \
    mt_checks++; \
    if (_a != _b) { mt_failed++; fprintf(stderr, "  FAIL %s:%d: %s == %s (%lld vs %lld)\n", \
                                         __FILE__, __LINE__, #a, #b, _a, _b); } \
} while (0)

#define RUN(fn) do { printf("  %-40s", #fn); int _f = mt_failed; fn(); puts(mt_failed == _f ? "ok" : "FAILED"); } while (0)

#define TEST_SUMMARY() (printf("  %d checks, %d failed\n", mt_checks, mt_failed), mt_failed ? EXIT_FAILURE : EXIT_SUCCESS)

#endif
