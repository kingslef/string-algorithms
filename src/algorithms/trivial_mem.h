#ifndef ALGORITHMS_TRIVIAL_MEM_H
#define ALGORITHMS_TRIVIAL_MEM_H

#include <stddef.h>
#include <stdint.h>

uint32_t trivial_mem_match(const char *text, const char *pattern,
                           const size_t text_len);

#endif /* ALGORITHMS_TRIVIAL_MEM_H */
