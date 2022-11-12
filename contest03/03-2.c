#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include <stdlib.h>

int
main(int argc, char *argv[]){
    int64_t sum_neg = 0, sum_pos = 0;

    for (int i = 1; i < argc; ++i) {

        char *eptr = NULL;
        errno = 0;
        int64_t tmp = strtol(argv[i], &eptr, 10);

        if (errno || *eptr || eptr == argv[i] || (int32_t) tmp != tmp) {
            fprintf(stderr, "INCORRECT INPUT.\n");
            return 1;
        }

        if (tmp > 0) {
            if (__builtin_add_overflow(tmp, sum_pos, &sum_pos)) {
                fprintf(stderr, "DATA IS TOO BIG\n");
                exit(1);
            }
        }
        if (tmp < 0) {
            if (__builtin_add_overflow(tmp, sum_neg, &sum_neg)) {
                fprintf(stderr, "DATA IS TOO BIG\n");
                exit(1);
            }
        }
    }
    printf("%" PRId64 "\n", sum_pos);
    printf("%" PRId64 "\n", sum_neg);

    return 0;
}
