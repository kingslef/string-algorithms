#include "rk.h"

#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

/* Based on pseudocode in T-106.5400 Course notes 2012. */
uint32_t rk_match(const char *text, const char *pattern, const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return 0;
    }

    uint32_t pattern_len = strlen(pattern);

    /* TODO: remove len checks */
    if (pattern_len > text_len || pattern_len == 0 || text_len == 0) {
        return 0;
    }

    if (pattern_len == 1 && text_len >= 1) {
        return (pattern[0] == text[0] ? 1 : 0);
    }

    uint32_t theta = 32;
    uint32_t q = (uint32_t)pow(2, 32);

    /* TODO: remove pow */
    uint32_t cm = (uint32_t)pow(theta, pattern_len - 1) % q;

    uint32_t hash_pattern = 0;
    uint32_t hash_text = 0;

    /* Calculate hash from pattern */
    for (uint32_t i = 0; i < pattern_len; i++) {
        hash_pattern = (hash_pattern * theta + pattern[i]) % q;
    }

    /* Calculate hash from pattern length substring starting from beginning */
    for (uint32_t i = 0; i < pattern_len; i++) {
        hash_text = (hash_text * theta + text[i]) % q;
    }

    uint32_t matched = 0;

    /* Check if calculated hashes match */
    if (hash_pattern == hash_text
        && strncmp(pattern, text, pattern_len) == 0) {
        /* Match */
        matched++;
        printf("rk: match at %u\n", 0);
    }

    /* Calculate rolling hash from rest of the text and compare it to
     * pattern */
    for (uint32_t i = 1; i < text_len - pattern_len + 1; i++) {

        /* New hash is calculated:
           - Subtract previous character times cm,
           - Multiply that by theta,
           - Add new character from the text,
           - All in mod q.
        */
        hash_text = ((hash_text - text[i - 1] * cm) * theta + text[i + pattern_len - 1]) % q;

        if (hash_pattern == hash_text
            && strncmp(pattern, text + i, pattern_len) == 0) {
            /* Match */
            matched++;
            printf("rk: match at %u\n", i);
        }
    }

    return matched;
}

