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

    return 0;
}

int kmp_match(const char *text, const char *pattern)
{
    (void)text;
    (void)pattern;

    return -1;
}
