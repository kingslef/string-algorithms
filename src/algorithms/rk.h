#ifndef ALGORITHMS_RK_H
#define ALGORITHMS_RK_H

#include <stddef.h>
#include <stdint.h>

uint32_t rk_match(const char *text, const char *pattern,
                  const size_t text_len);

#endif /* ALGORITHMS_RK_H */
