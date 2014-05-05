#include "shift.h"

#include <limits.h>
#include <string.h>

#include <math.h>

#define ALPHABET_LEN CHAR_MAX

int shift_precompute(const char *pattern, uint32_t *array)
{
    /* Let's assume that word size is 32 bits. */
    uint32_t pattern_len = strlen(pattern);

    if (pattern_len > 32) {
        return -1;
    }

    /* TODO: remove pow */
    uint32_t ones = (uint32_t)pow(2, pattern_len) - 1;

    /* Initialize all bits to one */
    for (uint32_t i = 0; i < pattern_len; i++) {
        array[i] = 1;
    }
}

int shift_match(const char *text, const char *pattern);
