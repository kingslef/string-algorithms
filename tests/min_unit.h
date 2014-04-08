#ifndef MIN_UNIT_H
#define MIN_UNIT_H

/* Idea from http://www.jera.com/techinfo/jtns/jtn002.html */

#define mu_assert_equal(message, test, i)                       \
    do {                                                        \
        int got = (test);                                       \
        int expected = (int)(i);                                \
        if (got != expected) {                                  \
            printf("%s:%d: %s: expected %d, got %d!\n",         \
                   __FILE__, __LINE__,__func__, expected, got); \
            return message;                                     \
        }                                                       \
    } while (0)

#define mu_run_test(test)                                \
    do {                                                 \
        const char *message = test();                    \
        tests_run++;                                     \
        if (message) {                                   \
            printf("%s:%d: test #%d failed!\n",          \
                   __FILE__, __LINE__, tests_run - 1);   \
            return message;                              \
        }                                                \
    } while (0)

#endif /* MIN_UNIT_H */
