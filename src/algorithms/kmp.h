#ifndef ALGORITHMS_KMP_H
#define ALGORITHMS_KMP_H

#include <stddef.h>
#include <stdint.h>

void kmp_build_border(const char *pattern, uint32_t *border,
                      const size_t pattern_len);

uint32_t kmp_match(const char *text, const char *pattern,
                   const size_t text_len);

#endif /* ALGORITHMS_KMP_H */
