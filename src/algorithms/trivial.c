#include "trivial.h"

#include <string.h>
#include <stdio.h>

/**
 * Find occurances of pattern in text using trivial algorithm.
 *
 * @returns number of occurances of pattern in text.
 */
uint32_t trivial_match(const char *text, const char *pattern,
                       const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return 0;
    }

    uint32_t matched = 0;
    for (size_t t = 0; t < text_len; t++) {
        size_t p;
        for (p = 0; pattern[p] != '\0'; p++) {
            if (t + p >= text_len || pattern[p] != text[t + p]) {
                break;
            }
        }
        if (pattern[p] == '\0') {
            /* Match */
            matched++;
            if (matched == 1) {
                printf("trivial: match at %zu", t);
            } else {
                printf(", %zu", t);
            }
        }
    }

    if (matched == 0) {
        printf("trivial: no matches\n");
    } else {
        putchar('\n');
    }

    return matched;
}
