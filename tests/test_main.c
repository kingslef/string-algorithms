#include <trivial.h>
#include <kmp.h>
#include <bm.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "min_unit.h"

static int tests_run;

static int find_needle(const char *haystack, const char *needle)
{
    char *f = strstr(haystack, needle);
    if (f == NULL) {
        return -1;
    }

    return (int)(f - haystack);
}

static const char *test_match_negative(int (*match_func)(const char*, const char*))
{
    mu_assert_equal("unexisting pattern was found", match_func("foo", "bar"), -1);
    mu_assert_equal("unexisting pattern was found", match_func("fo", "foo"), -1);
    mu_assert_equal("unexisting pattern was found", match_func("fo", NULL), -1);
    mu_assert_equal("unexisting pattern was found", match_func(NULL, NULL), -1);
    mu_assert_equal("unexisting pattern was found", match_func(NULL, "foo"), -1);

    return NULL;
}

static const char *test_match_positive(int (*match_func)(const char*, const char*))
{
    const char *pattern;
    const char *text;

    text = "foo";
    pattern = "foo";
    mu_assert_equal("pattern was not found", match_func(text, pattern), find_needle(text, pattern));

    text = "barfoo";
    pattern = "foo";
    mu_assert_equal("pattern was not found", match_func(text, pattern), find_needle(text, pattern));

    text = "barfoo rfoobarr foobar";
    pattern = "foobar";
    mu_assert_equal("pattern was not found", match_func(text, pattern), find_needle(text, pattern));

    text = "foobafoobafoobafoobafrboofarfoobar";
    pattern = "foobar";
    mu_assert_equal("pattern was not found", match_func(text, pattern), find_needle(text, pattern));

    text = "HOOWOOWOOHOOWOOWOO";
    pattern = "WOOHOO";
    mu_assert_equal("pattern was not found", match_func(text, pattern), find_needle(text, pattern));

    text = "SALLILAILLATAVANLALLILLALLALALLI";
    pattern = "LALLILLA";
    mu_assert_equal("pattern was not found", match_func(text, pattern), find_needle(text, pattern));


    return NULL;
}

static const char *kmp_border_test(const char *test_string,
                                   const int32_t correct_border[])
{
    const size_t string_len = strlen(test_string);
    int border[string_len];

    memset(border, 0, sizeof(border));

    mu_assert_equal("border creation failed",
                    kmp_build_border(test_string, border), 0);

    mu_assert_equal_array("wrong border array created",
                          border,
                          correct_border,
                          string_len);

    return NULL;
}

static const char *bm_good_suffix_test(const char *test_string,
                                       const int32_t correct_good_suffix[])
{
    const size_t string_len = strlen(test_string);
    int good_suffix[string_len + 1];

    memset(good_suffix, 0, sizeof(good_suffix));

    mu_assert_equal("good suffix creation failed",
                    bm_build_good_suffix(test_string, good_suffix), 0);

    mu_assert_equal_array("wrong good suffix array created",
                          good_suffix,
                          correct_good_suffix,
                          sizeof(good_suffix)/sizeof(good_suffix[0]));

    return NULL;
}


static const char *bm_bad_char_test(const char *pattern, const char *indexes,
                                    const uint32_t *values)
{
    uint32_t bad_char[CHAR_MAX];
    uint32_t bad_char_correct[CHAR_MAX];

    const uint32_t pattern_len = (uint32_t)strlen(pattern);

    for (uint32_t i = 0; i < CHAR_MAX; i++) {
        bad_char_correct[i] = pattern_len;
    }

    while (*indexes != '\0') {
        bad_char_correct[(int)*indexes] = *values;
        indexes++;
        values++;
    }

    mu_assert_equal("bad char array creation failed",
                    bm_build_bad_char(pattern, bad_char), 0);

    mu_assert_equal_array("wrong bad char rule array created",
                          bad_char,
                          bad_char_correct,
                          CHAR_MAX);

    return NULL;
}

static const char *test_strrnstr(void)
{
    const char* haystack;
    const char* needle;

    haystack = "foo";
    needle = "foo";
    mu_assert_equal_ptr("strrnstr failed",
                        strrnstr(haystack, needle, strlen(haystack)), haystack);

    haystack = "fofofo";
    needle = "fo";
    mu_assert_equal_ptr("strrnstr failed",
                        strrnstr(haystack, needle, strlen(haystack)),
                        haystack + strlen(haystack) - strlen(needle));

    haystack = "fofob";
    needle = "fo";
    mu_assert_equal_ptr("strrnstr failed",
                        strrnstr(haystack, needle, strlen(haystack)),
                        haystack + strlen(haystack) - strlen(needle) - 1);

    haystack = "fo";
    needle = "foo";
    mu_assert_equal_ptr("strrnstr succeeded",
                        strrnstr(haystack, needle, strlen(haystack)), NULL);

    haystack = "foo";
    needle = "b";
    mu_assert_equal_ptr("strrnstr succeeded",
                        strrnstr(haystack, needle, strlen(haystack)), NULL);

    return NULL;
}


static const char *all_tests(void)
{
    /* Trivial algorithm tests */
    mu_run_test(test_match_negative(trivial_match));
    mu_run_test(test_match_positive(trivial_match));

    /* strrnstr tests */
    mu_run_test(test_strrnstr());

    /* KMP border array tests */
    mu_run_test(kmp_border_test("ABCDABD",
                                (const int32_t []){0, 0, 0, 0, 1, 2, 0}));
    mu_run_test(kmp_border_test("ALABARALAALABARDA",
                                (const int32_t []){0, 0, 1, 0, 1, 0, 1, 2, 3, 1,
                                        2, 3, 4, 5, 6, 0, 1}));
    mu_run_test(kmp_border_test("TIPETIPETAP",
                                (const int32_t []){0, 0, 0, 0, 1, 2, 3, 4, 5, 0,
                                        0}));
    mu_run_test(kmp_border_test("ONEONETWO",
                                (const int32_t []){0, 0, 0, 1, 2, 3, 0, 0, 1}));
    mu_run_test(kmp_border_test("PARTICIPATE IN PARACHUTE",
                                (const int32_t []){0, 0, 0, 0, 0, 0, 0, 1, 2, 0,
                                        0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0,
                                        0}));

    /* KMP algorithm tests */
    mu_run_test(test_match_negative(kmp_match));
    mu_run_test(test_match_positive(kmp_match));

    /* BM bad character rule tests */
    mu_run_test(bm_bad_char_test("WOOHOO", "WOH", (const uint32_t []){5, 0, 2}));
    mu_run_test(bm_bad_char_test("LALLILLA", "AIL", (const uint32_t []){0, 3, 1}));
    mu_run_test(bm_bad_char_test("ABCDB", "ABCD", (const uint32_t []){4, 0, 2, 1}));
    mu_run_test(bm_bad_char_test("GCAGAGAG", "ACG", (const uint32_t []){1, 6, 0}));

    /* BM good suffix rule tests */
    mu_run_test(bm_good_suffix_test("abbabab",
                                    (const int32_t []){5, 5, 5, 5, 2, 5, 4, 1}));
    mu_run_test(bm_good_suffix_test("WOOHOO",
                                    (const int32_t []){6, 6, 6, 6, 3, 1, 1}));
    mu_run_test(bm_good_suffix_test("LALLILLA",
                                    (const int32_t []){6, 6, 6, 6, 6, 6, 6, 8, 1}));
    mu_run_test(bm_good_suffix_test("ABCDABC",
                                    (const int32_t []){4, 4, 4, 4, 4, 7, 7, 1}));
    /* TODO: check if correct */
    mu_run_test(bm_good_suffix_test("GCAGAGAG",
                                    (const int32_t []){7, 7, 7, 7, 2, 7, 4, 7, 1}));

    /* BM algorithm tests */
    mu_run_test(test_match_negative(bm_match));
    mu_run_test(test_match_positive(bm_match));

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
