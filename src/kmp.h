#ifndef ALGORITHMS_KMP_H
#define ALGORITHMS_KMP_H

#include <stdint.h>

int kmp_build_border(const char *pattern, int32_t *border);

int kmp_match(const char *text, const char *pattern);

#endif /* ALGORITHMS_KMP_H */
