#include "bm.h"

#include <limits.h>
#include <string.h>

/* TODO: remove */
#include <stdio.h>

#define DEBUG printf

#define ALPHABET_LEN CHAR_MAX

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

int bm_build_good_suffix(const char *pattern, int *good_suffix)
{
    if (pattern == NULL) {
        return -1;
    }

    /* TODO: remove strlen */
    const uint32_t pattern_len = (uint32_t)strlen(pattern);

    for (uint32_t i = 0; i < pattern_len; i++) {

    }

    return 0;
}

/* Graham A. Stephen: String Searching Algorithms */
int bm_build_bad_char(const char *pattern, uint32_t *bad_char)
{
    if (pattern == NULL) {
        return -1;
    }

    /* TODO: remove strlen */
    const uint32_t pattern_len = (uint32_t)strlen(pattern);

    /* Set everything to maximum value first */
    for (uint32_t i = 0; i < ALPHABET_LEN; i++) {
        bad_char[i] = pattern_len;
    }

    /* Set values in pattern */
    for (uint32_t i = 0; i < pattern_len; i++) {
        printf("%s: %u\n", __func__, i);
        bad_char[(int)pattern[i]] = pattern_len - i - 1;
        DEBUG("%s: set badchar[%c] to %u (i %u)\n", __func__, pattern[i],
              bad_char[(int)pattern[i]], i);
    }

    return 0;
}

/* Suitable if pattern is large
   [http://www.cs.uku.fi/~kilpelai/BSA05/lectures/slides03.pdf] */
int bm_match(const char *text, const char *pattern)
{
    if (text == NULL || pattern == NULL) {
        return -1;
    }

    /* TODO: remove strlen */
    const size_t pattern_len = strlen(pattern);
    const size_t text_len = strlen(text);

    uint32_t bad_char[ALPHABET_LEN] = {0};
    bm_build_bad_char(pattern, bad_char);

    /* TODO: add good suffix */
    int good_suffix[pattern_len + 1];
    bm_build_good_suffix(pattern, good_suffix);

    size_t i = pattern_len - 1;
    size_t j = pattern_len - 1;
    while (i < text_len) {
        while (text[i] == pattern[j]) {
            i--;
            j--;
            if (j == 0 && text[i] == pattern[j]) {
                /* Match */
                /* TODO: handle cast */
                return (int)i;
            }
        }
        i += MAX(bad_char[(int)text[i]], good_suffix[j]);
        j = pattern_len - 1;
    }

    return -1;
}
