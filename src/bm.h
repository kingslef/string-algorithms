#ifndef ALGORITHMS_BM_H
#define ALGORITHMS_BM_H

#include <stdint.h>
#include <stddef.h>

char *strrnstr(const char *haystack, const char *needle, const size_t haystack_len);

int bm_build_good_suffix(const char *pattern, int *good_suffix,
                         const size_t pattern_len);

int bm_build_bad_char(const char *pattern, uint32_t *bad_char,
                      const size_t pattern_len);

int bm_match(const char *text, const char *pattern, const size_t text_len);

#endif /* ALGORITHMS_BM_H */
