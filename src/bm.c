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

int bm_build_good_suffix(const char *pattern, int *good_suffix,
                         const size_t pattern_len)
{
    if (pattern == NULL) {
        return -1;
    }

    DEBUG("%s: building suffix for %s\n", __func__, pattern);

    good_suffix[pattern_len] = 1;

    /* Two cases: suffix occurs somewhere else in the pattern */
    for (uint32_t i = pattern_len - 2; ; i--) {
        const char *suffix_to_search = pattern + i + 1;

        DEBUG("%s: -- mismatch at %c (suffix %s)\n", __func__, pattern[i], suffix_to_search);

        uint32_t j = 0;
        const char *suffix;

        do {
            DEBUG("%s: searching from %s[0, %u)\n",
                  __func__, pattern, i - j);

            suffix = strrnstr(pattern, suffix_to_search, pattern_len - j);
            if (suffix) {
                DEBUG("%s: suffix %s found in %s[0, %u) at %u (from %s) -- %s\n",
                      __func__, suffix_to_search, pattern, i - j, suffix - pattern, suffix,
                      (suffix[-1] == pattern[i]) ? "but mismatch occurs" : "fine");
                if (suffix[-1] == pattern[i]) {
                    suffix = NULL;
                } else {
                    break;
                }
            } else {
                break;
            }
            j++;
            if (j >= i) {
                suffix = NULL;
                break;
            }
        } while (1);

        if (suffix) {
            good_suffix[i + 1] = (int)(suffix_to_search - suffix);
            DEBUG("%s: shifting %u by %u\n",
                  __func__, i + 1, good_suffix[i + 1]);
        } else {
            good_suffix[i + 1] = 0;
        }

        if (i == 0) {
            good_suffix[i] = 0;
            break;
        }
    }

    DEBUG("%s: --- first part done\n",
          __func__);

    for (uint32_t i = 0; i < pattern_len + 1; i++) {
        DEBUG("%u ", i);
    }
    DEBUG(":\n");

    for (uint32_t i = 0; i < pattern_len + 1; i++) {
        DEBUG("%u ", good_suffix[i]);
    }
    DEBUG("\n");


    /* ... or part of the suffix occurs at the beginning */
    for (uint32_t i = 0; i < pattern_len; i++) {
        uint32_t j = 0;

        int ret = 0;
        while (j != pattern_len) {
            if (j == 0 && i == 0) {
                j++;
            }

            ret = strncmp(pattern, pattern + i + j, pattern_len - i - j);
            if (ret == 0) {
                DEBUG("%s: suffix %s found\n", __func__, pattern + i + j);
                if (good_suffix[i] == 0) {
                    good_suffix[i] = i + j;
                    DEBUG("%s: shifting %u by %u\n",
                          __func__, i, good_suffix[i]);
                }
                break;
            } else {
                DEBUG("%s: suffix %s not found\n", __func__, pattern + i + j);
            }
            j++;
        }

        if (ret) {
            DEBUG("%s: no suffix found\n", __func__);
            if (good_suffix[i] == 0) {
                good_suffix[i] = pattern_len;
                DEBUG("%s: shifting %u by %u\n",
                      __func__, i, good_suffix[i]);
            }
        }

    }

    for (uint32_t i = 0; i < pattern_len + 1; i++) {
        DEBUG("%u ", good_suffix[i]);
    }
    DEBUG("\n");

    return 0;
}

/* Pseudocode from Graham A. Stephen: String Searching Algorithms */
int bm_build_bad_char(const char *pattern, uint32_t *bad_char,
                      const size_t pattern_len)
{
    if (pattern == NULL) {
        return -1;
    }

    /* Set everything to maximum value first */
    for (uint32_t i = 0; i < ALPHABET_LEN; i++) {
        bad_char[i] = pattern_len;
    }

    /* Set values in pattern */
    for (uint32_t i = 0; i < pattern_len; i++) {
        bad_char[(int)pattern[i]] = pattern_len - i - 1;
    }

    return 0;
}

/* Suitable if pattern is large
   [http://www.cs.uku.fi/~kilpelai/BSA05/lectures/slides03.pdf]

   Pseudocode from T-106.5400 Course notes 2012.
*/
int bm_match(const char *text, const char *pattern, const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return -1;
    }

    const size_t pattern_len = strlen(pattern);

    uint32_t bad_char[ALPHABET_LEN] = {0};
    bm_build_bad_char(pattern, bad_char, pattern_len);

    /* TODO: add good suffix */
    int good_suffix[pattern_len + 1];
    bm_build_good_suffix(pattern, good_suffix, pattern_len);

    size_t i = pattern_len - 1;
    size_t j = pattern_len - 1;

    int first_match = -1;
    while (i < text_len) {

        DEBUG("%s: checking from %zu\n",
              __func__, i);
        size_t start = i;

        while (text[i] == pattern[j]) {
            if (j == 0) {
                /* Match */
                if (first_match == -1) {
                    first_match = i;
                }
                printf("bm: match at %u\n", i);
                break;
            }

            i--;
            j--;
        }

        const uint32_t matched = (pattern_len - 1 - j);
        DEBUG("%s: matched %u\n", __func__, matched);

        DEBUG("%s: increasing i (=%u) by %u (bad_char %u, good_suffix %u)\n",
              __func__, i, MAX(bad_char[(int)text[i]] - matched, good_suffix[j+1]),
              bad_char[(int)text[i]] - matched, good_suffix[j+1]);
        i = start + MAX((int)bad_char[(int)text[i]] - (int)matched, good_suffix[j+1]);
        j = pattern_len - 1;
    }

    return first_match;
}
