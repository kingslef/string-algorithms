#ifndef ALGORITHMS_BM_H
#define ALGORITHMS_BM_H

#include <stdint.h>

int bm_build_good_suffix(const char *pattern, int *good_suffix);

int bm_build_bad_char(const char *pattern, uint32_t *bad_char);

int bm_match(const char *text, const char *pattern);

#endif /* ALGORITHMS_BM_H */
