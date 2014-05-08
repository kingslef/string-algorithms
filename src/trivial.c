#include "trivial.h"

#include <string.h>


/**
 * Searches for pattern in text using trivial algorithm.
 *
 * @returns -1 if no match or position in text where match starts.
 */
int trivial_match(const char *text, const char *pattern,
                  const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return -1;
    }

    for (size_t t = 0; t < text_len; t++) {
        size_t p;
        for (p = 0; pattern[p] != '\0'; p++) {
            if (t + p >= text_len || pattern[p] != text[t + p]) {
                break;
            }
        }
        if (pattern[p] == '\0') {
            /* TODO: fix cast */
            return (int)t;
        }
    }

    return -1;
}
