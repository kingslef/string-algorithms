#include "bm.h"
#include "rk.h"
#include "kmp.h"
#include "trivial.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <fcntl.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x)[0])

#define CALC_DIFF_MS(start, end) (((end).tv_sec * 10e2 + (end).tv_nsec * 10e-7) - \
                                  ((start).tv_sec * 10e2 + (start).tv_nsec * 10e-7))

static uint32_t (*match_funcs[])(const char*,
                                 const char*,
                                 const size_t) = { kmp_match, bm_match,
                                                   trivial_match,
                                                   rk_match };

static const char *match_func_names[ARRAY_LEN(match_funcs)] = { "kmp", "bm",
                                                                "trivial",
                                                                "rk" };

static uint32_t (*choose_best(const char *text, const char *pattern))(const char*,
                                                                      const char*,
                                                                      const size_t)
{
    /**
     * KMP: - Very bad when long pattern and match fails at the end.
     *      - Good when not much partial matches in text (better than BM).
     *
     * BM:  - Good when long pattern.
     *      - Good when pattern is not in the text.
     *      - Bad with very small (3 char) pattern and 6 char text.
     *
     * T:   - Good with ~20 char pattern and 100 lines.
     *
     * RK:  - Doesn't seem to be affected by much, it is always a bit slower than trivial.
     *
     */

    return kmp_match;
}

/**
 * It doesn't make sense to do this if the sampling size is small, since
 * algorithms with something to precomputate before checking matches will
 * suffer. Then again, if sampling size is large, we won't get any use of doing this.
 */
static uint32_t choose_best_by_sampling(const char *text, const char *pattern,
                                        const size_t text_len)
{
    const size_t pattern_len = strlen(pattern);
    const size_t sample_size = text_len / 4;


    if (sample_size < pattern_len * 2) {
        return 0;
    }

    double fastest_time = 0.0;
    int fastest_func = -1;

    for (uint32_t i = 0; i < ARRAY_LEN(match_funcs); i++) {
        struct timespec t_start = { 0, 0 };
        struct timespec t_end = { 0, 0 };

        clock_gettime(CLOCK_MONOTONIC, &t_start);

        match_funcs[i](text, pattern, sample_size);

        clock_gettime(CLOCK_MONOTONIC, &t_end);

        double run_time = CALC_DIFF_MS(t_start, t_end);

        printf("%s: %s took %5.2lf ms\n", __func__, match_func_names[i],
               run_time);

        if (fastest_func == -1 || run_time < fastest_time) {
            fastest_time = run_time;
            fastest_func = i;
        }
    }

    printf("%s chose %s\n", __func__, (fastest_func != -1 ? match_func_names[fastest_func] :
                                       "none"));

    return fastest_func;
}

static inline struct timespec calc_time(void)
{
    struct timespec t = { 0, 0 };

    clock_gettime(CLOCK_MONOTONIC, &t);

    return t;
}

int main(int argc, const char *argv[])
{
    int ret;
    const char *pattern;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <file_to_process>",
                argv[0]);
        return 1;
    }

    pattern = argv[1];

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

    struct timespec sampling_start = calc_time();

    uint32_t best_by_sampling = choose_best_by_sampling(text, pattern, text_size);

    struct timespec sampling_end = calc_time();

    double execution_times[ARRAY_LEN(match_funcs)];
    for (uint32_t i = 0; i < ARRAY_LEN(match_funcs); i++) {
        struct timespec time_start = calc_time();

        ret = match_funcs[i](text, pattern, text_size);

        struct timespec time_end = calc_time();

        execution_times[i] = CALC_DIFF_MS(time_start, time_end);
    }

    /* Report */
    int fastest_func = -1;
    for (uint32_t i = 0; i < ARRAY_LEN(match_funcs); i++) {
        printf("%10s: took %5.2lf ms (%+5.2lf ms to best by sampling)\n",
               match_func_names[i], execution_times[i],
               execution_times[i] - execution_times[best_by_sampling]);
        if (fastest_func == -1
            || execution_times[i] < execution_times[fastest_func]) {
            fastest_func = i;
        }
    }

    double diff = execution_times[best_by_sampling]
        - execution_times[fastest_func];

    double total = diff + CALC_DIFF_MS(sampling_start, sampling_end);
    printf("** Difference between fastest algorithm and algorithm chosen"
           " by sampling was:\n"
           "   %+.2lf ms + %.2lf ms (time took by sampling) = %+.2lf ms\n",
           diff, CALC_DIFF_MS(sampling_start, sampling_end), total);

    if (diff > 0.0) {
        printf(" => Not worth it\n");
    } else {
        printf(" => Worth it\n");
    }

    return 0;
}
