#include "bm.h"

#include <string.h>
#include <stdio.h>

/* TODO: remove */
#define DEBUG

#ifdef DEBUG
#include <time.h>
#define CALC_DIFF_MS(start, end) (((end).tv_sec * 10e2 + (end).tv_nsec * 10e-7) - \
                                  ((start).tv_sec * 10e2 + (start).tv_nsec * 10e-7))
#endif

#define MAX(x,y) ((x) > (y) ? (x) : (y))

/* Finds last occurance of needle in haystack */
char *strrnstr(const char *haystack, const char *needle, const size_t haystack_len)
{
    const size_t needle_len = strlen(needle);

    if (needle_len > haystack_len) {
        return NULL;
    }

    for (const char *str = haystack + haystack_len - needle_len;
         str >= haystack; str--) {
        int ret = strncmp(str, needle, needle_len);
        if (ret == 0) {
            return (char *)str;
        }
    }

    return NULL;
}

void bm_build_good_suffix(const char *pattern, int *good_suffix,
                          const size_t pattern_len)
{
    /* If the mismatch happens at the first character, we can only move by
     * one */
    good_suffix[pattern_len] = 1;

    /* First case, suffix occurs somewhere else in the pattern */
    for (uint32_t i = pattern_len - 2; ; i--) {
        const char *suffix_to_search = pattern + i + 1;

        uint32_t j = 0;

        /* Search suffix from the pattern, backwards */
        do {
            const char *suffix = strrnstr(pattern, suffix_to_search, pattern_len - j);

            if (suffix && suffix[-1] != pattern[i]) {
                /* Found */
                good_suffix[i + 1] = (int)(suffix_to_search - suffix);
                break;
            }

            j++;

            if (j >= i || suffix == NULL) {
                /* Not found */
                good_suffix[i + 1] = 0;
                break;
            }
        } while (1);

        if (i == 0) {
            good_suffix[0] = 0;
            break;
        }
    }

    /* Second case, part of the suffix occurs at the beginning */
    for (uint32_t i = 0; i < pattern_len; i++) {

        uint32_t j;
        for (j = 0; j < pattern_len; j++) {
            /* Skip first suffix */
            if (j == 0 && i == 0) {
                j++;
            }

            /* Search suffix from beginning but only check until to the point
             * where suffix starts */
            int ret = strncmp(pattern, pattern + i + j, pattern_len - i - j);
            if (ret == 0) {
                /* Found */
                if (good_suffix[i] == 0) {
                    good_suffix[i] = i + j;
                }
                break;
            }

        }

        if (j == pattern_len) {
            /* Not found */
            if (good_suffix[i] == 0) {
                good_suffix[i] = pattern_len;
            }
        }
    }

}

/* Pseudocode from Graham A. Stephen: String Searching Algorithms */
void bm_build_bad_char(const char *pattern, uint32_t *bad_char,
                       const size_t pattern_len)
{
    /* Set everything to maximum value first */
    for (uint32_t i = 0; i < ALPHABET_LEN; i++) {
        bad_char[i] = pattern_len;
    }

    /* Set values in pattern */
    for (uint32_t i = 0; i < pattern_len; i++) {
        bad_char[pattern[i]] = pattern_len - i - 1;
    }
}

/**
 * Find occurances of pattern in text using Boyer-Moore algorithm.
 *
 * @note Based on the pseudocode in T-106.5400 Course notes 2012, except that it
 * didn't calculate how many characters matched when using bad character rule
 * and that seemed to cause mismatches.
 *
 * @return number of occurances of pattern in text.
 */
uint32_t bm_match(const char *text, const char *pattern, const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return 0;
    }

    const size_t pattern_len = strlen(pattern);

#ifdef DEBUG
    struct timespec t_start = { 0, 0 };
    struct timespec t_end_bad = { 0, 0 };
    struct timespec t_end = { 0, 0 };

    clock_gettime(CLOCK_MONOTONIC, &t_start);
#endif

    uint32_t bad_char[ALPHABET_LEN] = {0};
    bm_build_bad_char(pattern, bad_char, pattern_len);

#ifdef DEBUG
    clock_gettime(CLOCK_MONOTONIC, &t_end_bad);
#endif

    int good_suffix[pattern_len + 1];
    bm_build_good_suffix(pattern, good_suffix, pattern_len);

#ifdef DEBUG
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    printf("%s: Preprocessing took %.2lf (%.2lf + %.2lf) ms\n",
           __func__, CALC_DIFF_MS(t_start, t_end),
           CALC_DIFF_MS(t_start, t_end_bad), CALC_DIFF_MS(t_end_bad, t_end));
#endif

    size_t i = pattern_len - 1;
    size_t j = pattern_len - 1;

    uint32_t matched = 0;
    while (i < text_len) {
        size_t start = i;

        while (text[i] == pattern[j]) {
            if (j == 0) {
                /* Match */
                matched++;
                if (matched == 1) {
                    printf("bm: match at %zu", i);
                } else {
                    printf(", %zu", i);
                }
                break;
            }

            i--;
            j--;
        }

        const uint32_t matched_chars = (pattern_len - 1 - j);
        i = start + MAX((int)bad_char[text[i]] - (int)matched_chars,
                        good_suffix[j+1]);
        j = pattern_len - 1;
    }

    if (matched == 0) {
        printf("bm: no matches\n");
    } else {
        putchar('\n');
    }

    return matched;
}
