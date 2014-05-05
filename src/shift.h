#ifndef ALGORITHMS_SHIFT_H
#define ALGORITHMS_SHIFT_H

#include <stdint.h>

int shift_precompute(const char *pattern, uint32_t *array);

int shift_match(const char *text, const char *pattern);

#endif /* ALGORITHMS_SHIFT_H */
