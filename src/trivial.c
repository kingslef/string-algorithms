#include "trivial.h"

#include <string.h>


/**
 * Searches for pattern in text using trivial algorithm.
 *
 * @returns -1 if no match or position in text where match starts.
 */
int trivial_match(const char *text, const char *pattern)
{
    if (text == NULL || pattern == NULL) {
        return -1;
    }

    for (size_t t = 0; text[t] != '\0'; t++) {
        size_t p;
        for (p = 0; pattern[p] != '\0'; p++) {
            if (text[t + p] == '\0' || pattern[p] != text[t + p]) {
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
