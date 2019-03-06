/* file: microunit.h
 * based on: http://www.jera.com/techinfo/jtns/jtn002.html
 *
 * - added mu_assert_1 and mu_assert_2
 * - added header guard
 * - made const correct
 * - made tests_run counter atomic
 * - syntactically supports C89 and C++98.
 * - TODO: add nonportable hacks for C++98 and C99 and below
 * -       they lack a language level way to indicate that something is atomic
 * -       so getting atomicity here will require inline assembly and ifdef soup
 * - TODO: better story for running tests in parallel
 *
 *     */

#ifndef MICROUNIT_MICROUNIT_H_HGUARD
#define MICROUNIT_MICROUNIT_H_HGUARD

/* BEGIN SELF TEST
 * These are self-contained examples that document the usage of the public API
 * They are also Microtest's test suite
 * MICU_ is pronounced /mi.tʃu/ or /mi.ku/ */

#define MICROUNIT_SELFTEST_1_PROG          \
    MICU_DEFCOUNTER_1(tests_run);          \
                                           \
    const char* test_sum(void);            \
    const char* all_tests(void);           \
                                           \
    const char* test_sum()                 \
    {                                      \
        MICU_ASSERT_1(1 + 1 == 2);         \
        MICU_ASSERT_1(0 + 1 == 1);         \
        return NULL;                       \
    }                                      \
                                           \
    const char* all_tests()                \
    {                                      \
        MICU_RUN_TEST_1(test_sum);         \
        return NULL;                       \
    }                                      \
                                           \
    int main()                             \
    {                                      \
        /* note that the count refers      \
         * to the number of tests          \
         * not the number of assertions */ \
        MICU_RETURN_MAIN_2(all_tests, 1);  \
    }

/* END SELF TEST */

#undef MICU_ATOMIC_INT_T
#ifdef __cplusplus

#include <atomic>
#include <cstdio>

#define MICU_ATOMIC_INT_T std::atomic<int>

/* note: a global std::atomic<int> will get a value of 0
 * rather than uninitialized memory */
#define MICU_DEFCOUNTER_1(var) \
    MICU_ATOMIC_INT_T var

#define MICU_ATOMIC_INCREMENT(X) \
    do {                         \
        (X)++;                   \
    } while (0)

#define MICU_READ_ATOMIC(var) \
    ((var).load())

#else

#define MICU_ATOMIC_INT_T atomic_int

#define MICU_DEFCOUNTER_1(var) \
    MICU_ATOMIC_INT_T var = (0)

#define MICU_READ_ATOMIC(var) \
    (atomic_load(&(var)))

#include <stdatomic.h>

#include <stdio.h>

#define MICU_ATOMIC_INCREMENT(X)                                  \
    do {                                                          \
        atomic_fetch_add_explicit(&(X), 1, memory_order_relaxed); \
    } while (0)
#endif

extern MICU_ATOMIC_INT_T tests_run;

#define MICU_ASSERT_1(test) \
    MICU_ASSERT_2((#test), (test))

#define MICU_ASSERT_2(message, test) \
    do {                             \
        if (!(test)) {               \
            return (message);        \
        }                            \
    } while (0)

#define MICU_RUN_TEST_1(test)               \
    do {                                    \
        const char* message;                \
        message = test();                   \
        MICU_ATOMIC_INCREMENT(tests_run);   \
        if (message) {                      \
            printf("not ok - " #test "\n"); \
            return (message);               \
        } else {                            \
            printf("ok\n");                 \
        }                                   \
    } while (0)

#define MICU_RETURN_MAIN_2(FUNC, NUM)                                                                 \
    do {                                                                                              \
        const char* result;                                                                           \
        printf("1.." #NUM "\n");                                                                      \
        result = FUNC();                                                                              \
        if (!result) {                                                                                \
            if (MICU_READ_ATOMIC(tests_run) == (NUM)) {                                               \
                printf("# PASS\n");                                                                   \
                printf("# Tests run: %d\n", MICU_READ_ATOMIC(tests_run));                             \
                return 0;                                                                             \
            } else {                                                                                  \
                printf("# FAIL -- BAD COUNT\n");                                                      \
                printf("# Tests expected: %d , Tests run: %d\n", (NUM), MICU_READ_ATOMIC(tests_run)); \
                return 2;                                                                             \
            }                                                                                         \
        } else {                                                                                      \
            printf("# %s\n", result);                                                                 \
            printf("# FAIL -- TEST FAILURE\n");                                                       \
            return 1;                                                                                 \
        }                                                                                             \
    } while (0)

#endif /* MICROUNIT_MICROUNIT_H_HGUARD */
