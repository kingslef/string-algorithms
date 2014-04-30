#include "bm.h"

#include <limits.h>
#include <string.h>

/* TODO: remove */
#include <stdio.h>

#define DEBUG printf

#define ALPHABET_LEN CHAR_MAX

int bm_build_good_suffix(const char *pattern, int *good_suffix);

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
int bm_match(const char *text, const char *pattern);
