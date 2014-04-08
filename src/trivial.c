#include "trivial.h"

#include <string.h>


/**
 * Searches for pattern in text using trivial algorithm.
 *
 * @returns -1 if no match or position in text where match starts.
 */
int trivial_match(const char *text, const char *pattern)
{
    if (!text || !pattern) {
        return -1;
    }

    /* TODO: remove strlen and replace with '\0' checks */
    const size_t text_len = strlen(text);
    const size_t pattern_len = strlen(pattern);

    if (text_len < pattern_len) {
        return -1;
    }

    for (size_t t = 0; t <= text_len - pattern_len; t++) {
        size_t p;
        for (p = 0; p < pattern_len; p++) {
            if (pattern[p] != text[t + p]) {
                break;
            }
        }
        if (p == pattern_len) {
            /* TODO: fix cast */
            return (int)t;
        }
    }

    return -1;
}
