#include "kmp.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

int kmp_build_border(const char *pattern, uint32_t *border,
                     const size_t pattern_len)
{
    if (border == NULL || pattern == NULL || border == NULL) {
        return -1;
    }

    if (pattern_len > 0) {
        border[0] = 0;
    }

    int candidate = 0;

    for (size_t i = 1; i < pattern_len;) {
        if (pattern[i] == pattern[candidate]) {
            candidate++;
        } else if (candidate > 0) {
            /* No match, continue to previous border */
            candidate = border[candidate];
            continue;
        }

        border[i] = candidate;
        i++;
    }

    /* TODO: remove return value */
    return 0;
}

/**
 * Find occurances of pattern in text using Knuth-Morris-Pratt algorithm.
 *
 * @note Based on pseudocode in Graham A. Stephen, String searching algorithms,
 * except that it didn't quite support multiple matches.
 *
 * @return number of occurances of pattern in text.
 */
uint32_t kmp_match(const char *text, const char *pattern,
                   const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return 0;
    }

    const size_t pattern_len = strlen(pattern);

    if (pattern_len == 0) {
        return 0;
    }

    uint32_t border[pattern_len];

    kmp_build_border(pattern, border, pattern_len);

    uint32_t i = 0;
    uint32_t j = 0;

    uint32_t matched = 0;
    while (i < text_len && j < pattern_len) {
        while (text[i] != pattern[j]) {
            if (j == 0) {
                break;
            }
            j = border[j - 1];
        }
        i++;
        j++;
        if (j == pattern_len) {
            /* Match */
            matched++;
            uint32_t match_pos = i - pattern_len;
            if (matched == 1) {
                printf("kmp: match at %u", match_pos);
            } else {
                printf(", %u", match_pos);
            }
            j = border[j - 1];
        }
    }

    if (matched == 0) {
        printf("kmp: no matches\n");
    } else {
        putchar('\n');
    }

    return matched;
}
