#include <trivial.h>

#include <stdio.h>
#include <string.h>

#include "min_unit.h"

static int tests_run;

static const char *test_trivial_negative(void)
{
    mu_assert_equal("unexisting pattern was found", trivial_match("foo", "bar"), -1);
    mu_assert_equal("unexisting pattern was found", trivial_match("fo", "foo"), -1);
    mu_assert_equal("unexisting pattern was found", trivial_match("fo", NULL), -1);
    mu_assert_equal("unexisting pattern was found", trivial_match(NULL, NULL), -1);
    mu_assert_equal("unexisting pattern was found", trivial_match(NULL, "foo"), -1);
    mu_assert_equal("unexisting pattern was found", trivial_match(NULL, "foo"), -1);

    return NULL;
}

static const char *test_trivial_positive(void)
{
    mu_assert_equal("pattern was not found", trivial_match("foo", "foo"), 0);
    mu_assert_equal("pattern was not found", trivial_match("barfoo", "foo"),
                    strlen("bar"));
    mu_assert_equal("pattern was not found",
                    trivial_match("barfoo rfoobarr foobar", "foobar"),
                    strlen("barfoo r"));

    return NULL;
}

static const char *all_tests(void)
{
    mu_run_test(test_trivial_negative);
    mu_run_test(test_trivial_positive);

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

