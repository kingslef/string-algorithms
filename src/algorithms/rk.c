#include "rk.h"

#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

/* Prime number used in hashing in Rabin-Karp */
#define PRIME 33554393

/**
 * Find occurances of pattern in text using Rabin-Karp algorithm.
 *
 * @note Based on the pseudocode in T-106.5400 Course notes 2012.
 *
 * @return number of occurances of pattern in text or zero if pattern_len is too
 * long for constant values.
 */
uint32_t rk_match(const char *text, const char *pattern, const size_t text_len)
{
    if (text == NULL || pattern == NULL) {
        return 0;
    }

    uint32_t pattern_len = strlen(pattern);

    if (pattern_len > text_len || pattern_len == 0 || text_len == 0) {
        return 0;
    }

    if (pattern_len == 1 && text_len >= 1) {
        return (pattern[0] == text[0] ? 1 : 0);
    }

    const uint32_t theta = 32;
    const uint32_t q = PRIME;

    /* Other than this we don't need any casts to long because pattern times
       theta plus a character cannot be greater than 32bit unsigned integer */
    const uint32_t cm = ((unsigned long)pow(theta, pattern_len - 1)) % q;

    if (cm == 0) {
        /* theta ^ pattern_len probably overflowed, so pattern_len is too
         * long */
        printf("rk: too long pattern!\n");
        return 0;
    }

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
        printf("rk: match at %u", 0);
    }

    /* Calculate rolling hash from rest of the text and compare it to
     * pattern */
    for (uint32_t i = 1; i < text_len - pattern_len + 1; i++) {
        /* New hash is calculated:
           - Subtract previous character times cm,
           - Multiply that by theta,
           - Add new character from the text,
           - All in mod q. */
        hash_text = ((hash_text - text[i - 1] * cm) * theta + text[i + pattern_len - 1]) % q;

        if (hash_pattern == hash_text
            && strncmp(pattern, text + i, pattern_len) == 0) {
            /* Match */
            matched++;
            if (matched == 1) {
                printf("rk: match at %u", i);
            } else {
                printf(", %u", i);
            }
        }
    }

    if (matched == 0) {
        printf("rk: no matches\n");
    } else {
        putchar('\n');
    }

    return matched;
}
