#include <bm.h>
#include <rk.h>
#include <kmp.h>
#include <trivial.h>

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x)[0])

static int (*match_funcs[])(const char*,
                            const char*,
                            const size_t) = { kmp_match, bm_match,
                                              trivial_match,
                                              rk_match };

static const char *match_func_names[ARRAY_LEN(match_funcs)] = { "kmp", "bm",
                                                                "trivial",
                                                                "rk" };

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
        perror("open failed");
        return 1;
    }

    struct stat statbuf;

    ret = fstat(fd, &statbuf);
    if (ret < 0) {
        perror("fstat failed");
        return 1;
    }

    const char *text = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (text == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    for (uint32_t i = 0; i < ARRAY_LEN(match_funcs); i++) {
        ret = match_funcs[i](text, argv[1], statbuf.st_size);
        if (ret < 0) {
            printf("%s: pattern '%s' not found\n", match_func_names[i], argv[1]);
        } else {
            printf("%s: pattern '%s' found in %d\n", match_func_names[i], argv[1], ret);
        }
    }

    return 0;
}
