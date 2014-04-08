#include "kmp.h"

#include <string.h>

int kmp_build_border(const char *pattern, int32_t *border)
{
    /* TODO: remove strlen */
    size_t len = strlen(pattern);
    int32_t candidate = 0;

    if (len > 0) {
        border[0] = 0;
    }

    for (size_t i = 1; i < len;) {
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

int kmp_match(const char *text, const char *pattern)
{
    if (text == NULL || pattern == NULL) {
        return -1;
    }

    int32_t border[strlen(pattern) + 1];

    kmp_build_border(pattern, &(border[1]));

    border[0] = -1;

    const size_t text_len = strlen(text);
    const size_t pattern_len = strlen(pattern);

    uint32_t i = 0;
    uint32_t match = 0;

    while (match + i < text_len) {
        if (pattern[i] == text[match + i]) {
            i++;
            if (i == pattern_len) {
                return (int)match;
            }
        } else {
            match = match + i - border[i];
            if (border[i] >= 0) {
                i = (uint32_t)border[i];
            } else {
                i = 0;
            }
        }
    }

    return -1;
}
