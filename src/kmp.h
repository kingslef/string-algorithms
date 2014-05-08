#ifndef ALGORITHMS_KMP_H
#define ALGORITHMS_KMP_H

#include <stddef.h>

int kmp_build_border(const char *pattern, int *border,
                     const size_t pattern_len);

int kmp_match(const char *text, const char *pattern);

#endif /* ALGORITHMS_KMP_H */
