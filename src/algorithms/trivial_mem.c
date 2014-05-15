#include "trivial_mem.h"

#include <string.h>
#include <stdio.h>

/**
 * Find occurances of pattern in text using memmem(2).
 *
 * @note Got an idea for this from freebsd mailing list discussion "why GNU grep
 * is fast".
 *
 * @return number of occurances of pattern in text.
 */
uint32_t trivial_mem_match(const char *text, const char *pattern,
                           const size_t text_len)
{
    if (pattern == NULL || text == NULL) {
        return 0;
    }

    const size_t pattern_len = strlen(pattern);

    const char *haystack = text;
    size_t haystack_len = text_len;
    uint32_t matched = 0;

    while (haystack - text < text_len) {
        const char *match = memmem(haystack, haystack_len, pattern, pattern_len);
        if (match == NULL) {
            break;
        } else {
            /* Match */
            matched++;
            const uint32_t match_pos = match - text;
            if (matched == 1) {
                printf("trivial_mem: match at %u", match_pos);
            } else {
                printf(", %u", match_pos);
            }
            /* Skip previous match */
            haystack = match + 1;
            haystack_len = text_len - (haystack - text);
        }
    }

    if (matched == 0) {
        printf("trivial_mem: no matches\n");
    } else {
        putchar('\n');
    }

    return matched;
} 
