#include <trivial.h>
#include <kmp.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "min_unit.h"

static int tests_run;

static const char *test_match_negative(int (*match_func)(const char*, const char*))
{
    mu_assert_equal("unexisting pattern was found", match_func("foo", "bar"), -1);
    mu_assert_equal("unexisting pattern was found", match_func("fo", "foo"), -1);
    mu_assert_equal("unexisting pattern was found", match_func("fo", NULL), -1);
    mu_assert_equal("unexisting pattern was found", match_func(NULL, NULL), -1);
    mu_assert_equal("unexisting pattern was found", match_func(NULL, "foo"), -1);
    mu_assert_equal("unexisting pattern was found", match_func(NULL, "foo"), -1);

    return NULL;
}

static const char *test_match_positive(int (*match_func)(const char*, const char*))
{
    mu_assert_equal("pattern was not found", match_func("foo", "foo"), 0);
    mu_assert_equal("pattern was not found", match_func("barfoo", "foo"),
                    strlen("bar"));
    mu_assert_equal("pattern was not found",
                    match_func("barfoo rfoobarr foobar", "foobar"),
                    strlen("barfoo r"));

    return NULL;
}

static const char *kmp_border_test(const char *test_string, uint32_t string_len,
                                   const int32_t correct_border[])
{
    int32_t border[string_len + 1];

    mu_assert_equal("border creation failed",
                    kmp_build_border(test_string, border), 0);

    mu_assert_equal_array("wrong border array created",
                          border,
                          correct_border,
                          string_len);

    return NULL;
}


static const char *all_tests(void)
{
    /* Trivial algorithm tests */
    mu_run_test(test_match_negative(trivial_match));
    mu_run_test(test_match_positive(trivial_match));

    /* KMP border array tests */
    mu_run_test(kmp_border_test("ABCDABD", strlen("ABCDABD"),
                                (const int32_t []){0, 0, 0, 0, 1, 2, 0}));
    mu_run_test(kmp_border_test("ALABARALAALABARDA",
                                strlen("ALABARALAALABARDA"),
                                (const int32_t []){0, 0, 1, 0, 1, 0, 1, 2, 3, 1,
                                        2, 3, 4, 5, 6, 0, 1}));
    mu_run_test(kmp_border_test("TIPETIPETAP", strlen("TIPETIPETAP"),
                                (const int32_t []){0, 0, 0, 0, 1, 2, 3, 4, 5, 0,
                                        0}));
    mu_run_test(kmp_border_test("ONEONETWO", strlen("ONEONETWO"),
                                (const int32_t []){0, 0, 0, 1, 2, 3, 0, 0, 1}));
    mu_run_test(kmp_border_test("PARTICIPATE IN PARACHUTE",
                                strlen("PARTICIPATE IN PARACHUTE"),
                                (const int32_t []){0, 0, 0, 0, 0, 0, 0, 1, 2, 0,
                                        0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0,
                                        0}));

    /* KMP algorithm tests */
    mu_run_test(test_match_negative(kmp_match));
    mu_run_test(test_match_positive(kmp_match));

    return NULL;
}


int main(void)
{
    const char *result = all_tests();

    printf("Tests run: %d\n", tests_run);

    if (result) {
        printf("Test failed: %s\n", result);
        return -1;
    }

    printf("All tests passed\n");
    return 0;
}
