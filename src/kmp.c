#include "kmp.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

int kmp_build_border(const char *pattern, int *border,
                     const size_t pattern_len)
{
    if (border == NULL || pattern == NULL) {
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

uint32_t kmp_match(const char *text, const char *pattern,
                   const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return 0;
    }

    const size_t pattern_len = strlen(pattern);

    int border[pattern_len + 1];

    kmp_build_border(pattern, &(border[1]), pattern_len);

    border[0] = -1;

    uint32_t i = 0;
    uint32_t match = 0;

    uint32_t matched = 0;
    while (match + i < text_len) {
        if (pattern[i] == text[match + i]) {
            i++;
            if (i == pattern_len) {
                /* Match */
                matched++;
                printf("kmp: match at %u\n", match);
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

    return matched;
}
