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

#define mu_assert_equal_array(message, got, expected, size)             \
    do {                                                                \
        for (size_t i = 0; i < (size); i++) {                           \
            if ((got)[i] != (expected)[i]) {                            \
                printf("%s:%d: %s: expected[%zu] %d, got[%zu] %d!\n",   \
                       __FILE__, __LINE__, __func__,                    \
                       i, (expected)[i], i, (got)[i]);                  \
                printf("%s:%d: %s: got: ",                              \
                       __FILE__, __LINE__, __func__);                   \
                for (i = 0; i < (size); i++)                            \
                    printf("%d ", (got)[i]);                            \
                putchar('\n');                                          \
                printf("%s:%d: %s: expected: ",                         \
                       __FILE__, __LINE__, __func__);                   \
                for (i = 0; i < (size); i++)                            \
                    printf("%d ", (expected)[i]);                       \
                putchar('\n');                                          \
                return (message);                                       \
            }                                                           \
        }                                                               \
    } while (0)


#define mu_run_test(test)                                \
    do {                                                 \
        const char *message = test;                      \
        tests_run++;                                     \
        if (message) {                                   \
            printf("%s:%d: test #%d failed!\n",          \
                   __FILE__, __LINE__, tests_run - 1);   \
            return message;                              \
        }                                                \
    } while (0)

#endif /* MIN_UNIT_H */
