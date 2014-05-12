#include "bm.h"
#include "rk.h"
#include "kmp.h"
#include "trivial.h"

#include <stdio.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <fcntl.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x)[0])

#define CALC_DIFF_MS(start, end) (((end).tv_sec * 10e2 + (end).tv_nsec * 10e-7) - \
                                  ((start).tv_sec * 10e2 + (start).tv_nsec * 10e-7))


static int (*match_funcs[])(const char*,
                            const char*,
                            const size_t) = { kmp_match, bm_match,
                                              trivial_match,
                                              rk_match };

static const char *match_func_names[ARRAY_LEN(match_funcs)] = { "kmp", "bm",
                                                                "trivial",
                                                                "rk" };

static int (*choose_best(const char *text, const char *pattern))(const char*,
                                                                 const char*,
                                                                 const size_t)
{
    /**
     * KMP: - Very bad when long pattern and match fails at the end.
     *      - Good when not much partial matches in text (better than BM).
     *
     * BM:  - Good when long pattern.
     *      - Good when pattern is not in the text.
     *
     * T:   - Good with ~20 char pattern and 100 lines.
     *
     * RK:  - Doesn't seem to be affected by much.
     *
     */

    return kmp_match;
}

int main(int argc, const char *argv[])
{
    int ret;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <pattern> <files_to_process>",
                argv[0]);
        return 1;
    }

    int fd = open(argv[2], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "open for %s failed: %s\n", argv[2], strerror(fd));
        return 1;
    }

    struct stat statbuf;
    ret = fstat(fd, &statbuf);
    if (ret < 0) {
        fprintf(stderr, "fstat for %s failed: %s\n", argv[2], strerror(fd));
        return 1;
    }

    size_t text_size = (size_t)statbuf.st_size;

    const char *text = mmap(NULL, text_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (text == MAP_FAILED) {
        fprintf(stderr, "mmap for %s failed: %s\n", argv[2], strerror(fd));
        return 1;
    }

    for (uint32_t i = 0; i < ARRAY_LEN(match_funcs); i++) {
        struct timespec t_start = { 0, 0 };
        struct timespec t_end = { 0, 0 };

        clock_gettime(CLOCK_MONOTONIC, &t_start);

        ret = match_funcs[i](text, argv[1], text_size);

        clock_gettime(CLOCK_MONOTONIC, &t_end);

        printf("%7s: '%s' %s %d. It took %5.2lf ms\n", match_func_names[i],
               argv[1], (ret < 0 ? "not found:" : "found in"),
               ret, CALC_DIFF_MS(t_start, t_end));
    }

    return 0;
}
