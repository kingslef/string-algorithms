#include "bm.h"
#include "rk.h"
#include "kmp.h"
#include "trivial.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <fcntl.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x)[0])

#define CALC_DIFF_MS(start, end) (((end).tv_sec * 10e2 + (end).tv_nsec * 10e-7) - \
                                  ((start).tv_sec * 10e2 + (start).tv_nsec * 10e-7))

typedef struct {
    const char *name;
    uint32_t (*func)(const char *, const char *, const size_t);
} string_algorithm_t;

static string_algorithm_t algorithms[] = {
    { .name = "kmp", .func = kmp_match },
    { .name = "bm", .func = bm_match },
    { .name = "trivial", .func = trivial_match },
    { .name = "rk", .func = rk_match }
};

enum algorithms_t {
    kmp,
    bm,
    trivial,
    rk,
    end
};

static inline struct timespec get_time(void)
{
    struct timespec t = { 0, 0 };

    clock_gettime(CLOCK_MONOTONIC, &t);

    return t;
}

static enum algorithms_t choose_best_by_analyzing(const char *text,
                                                  const char *pattern,
                                                  const size_t text_len,
                                                  double *time_taken)
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

    struct timespec start_time = get_time();
    const size_t pattern_len = strlen(pattern);

    /* If text is quite short, it is fastest
       use trivial algorithm */
    if (text_len <= 10000) {
        return trivial;
    }

    /* If pattern is not too long but text is large,
       bm should be fastest */
    if (pattern_len <= 200 && text_len >= 10000) {
        return bm;
    }

    *time_taken = CALC_DIFF_MS(start_time, get_time());

    return rk;
}

/**
 * It doesn't make sense to do this if the sampling size is small, since
 * algorithms with something to precomputate before checking matches will
 * suffer. Then again, if sampling size is large, we won't get any use of doing this.
 */
static enum algorithms_t choose_best_by_sampling(const char *text,
                                                 const char *pattern,
                                                 const size_t text_len,
                                                 double *time_taken)
{
    struct timespec start_time = get_time();
    const size_t pattern_len = strlen(pattern);
    const size_t sample_size = text_len / 4;


    if (sample_size < pattern_len * 2) {
        return trivial;
    }

    double fastest_time = 0.0;
    enum algorithms_t fastest_func = end;

    for (enum algorithms_t a = kmp; a != end; a++) {
        struct timespec t_start = get_time();

        algorithms[a].func(text, pattern, sample_size);

        double run_time = CALC_DIFF_MS(t_start, get_time());

        if (fastest_func == end || run_time < fastest_time) {
            fastest_time = run_time;
            fastest_func = a;
        }
    }

    *time_taken = CALC_DIFF_MS(start_time, get_time());

    return fastest_func;
}

/**
 * Read file to memory with mmap.
 *
 * @return memory location returned by mmap and file size in @param filesize.
 */
static const char *read_file_to_mem(const char *filename, size_t *filesize)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "open for %s failed: %s\n", filename, strerror(fd));
        exit(EXIT_FAILURE);
    }

    struct stat filestat;
    int ret = fstat(fd, &filestat);
    if (ret < 0) {
        fprintf(stderr, "fstat for %s failed: %s\n", filename, strerror(fd));
        exit(EXIT_FAILURE);
    }

    const char *file = mmap(NULL, filestat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED) {
        fprintf(stderr, "mmap for %s failed: %s\n", filename, strerror(fd));
        exit(EXIT_FAILURE);
    }

    if (filesize != NULL) {
        *filesize = (size_t)filestat.st_size;
    }

    return file;
}

/**
 * Search pattern from text using all algorithms defined in algorithms array.
 *
 * @note There should be enough space in @param execution_times.
 *
 * @return execution times of each algorithm in @param execution_times.
 */
static void find_pattern(const char *text, const size_t text_len,
                         const char *pattern, double execution_times[])
{
    for (enum algorithms_t a = kmp; a != end; a++) {
        struct timespec time_start = get_time();

        algorithms[a].func(text, pattern, text_len);

        struct timespec time_end = get_time();

        execution_times[a] = CALC_DIFF_MS(time_start, time_end);
    }
}

int main(int argc, const char *argv[])
{
    const char *pattern;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <file_to_process>",
                argv[0]);
        return 1;
    }

    pattern = argv[1];

    size_t text_size;
    const char *text = read_file_to_mem(argv[2], &text_size);

    double sampling_time;
    enum algorithms_t best_by_sampling = choose_best_by_sampling(text, pattern,
                                                                 text_size,
                                                                 &sampling_time);
    double analyzing_time;
    enum algorithms_t best_by_analyzing = choose_best_by_analyzing(text,
                                                                   pattern,
                                                                   text_size,
                                                                   &analyzing_time);

    double execution_times[ARRAY_LEN(algorithms)];
    find_pattern(text, text_size, pattern, execution_times);

    /* Report */
    enum algorithms_t fastest = end;
    for (enum algorithms_t a = kmp; a != end; a++) {
        printf("%-10s %5.2lf ms\n"
               "\t(%+5.2lf ms to sampling)\n"
               "\t(%+5.2lf ms to analyzing)\n\n",
               algorithms[a].name, execution_times[a],
               execution_times[a] - execution_times[best_by_sampling],
               execution_times[a] - execution_times[best_by_analyzing]);

        if (fastest == end
            || execution_times[a] < execution_times[fastest]) {
            fastest = a;
        }
    }

    double diff = execution_times[best_by_sampling]
        - execution_times[fastest];

    double total = diff + sampling_time;
    printf("** Difference between fastest algorithm and algorithm chosen"
           " by sampling was:\n"
           "   %+.2lf ms + %.2lf ms (time took by sampling per algorithm) = %+.2lf ms\n",
           diff, sampling_time, total);

    /* Couple of millisecond difference should be acceptable */
    /* TODO: This comparison is not very fair because it is affected by how many
     * algorithms we have */
    if (total / ARRAY_LEN(algorithms) > 3.0) {
        printf(" => Not worth it\n");
    } else {
        printf(" => Worth it\n");
    }

    putchar('\n');

    diff = execution_times[best_by_analyzing]
        - execution_times[fastest];

    total = diff + analyzing_time;
    printf("** Difference between fastest algorithm and algorithm chosen"
           " by analyzing was:\n"
           "   %+.2lf ms + %.2lf ms (time took by analyzing) = %+.2lf ms\n",
           diff, analyzing_time, total);

    /* Couple millisecond difference should be acceptable */
    if (total > 3.0) {
        printf(" => Not worth it\n");
    } else {
        printf(" => Worth it\n");
    }

    printf("\nText length was %zu and pattern length was %zu characters\n",
           text_size, strlen(pattern));

    return 0;
}
