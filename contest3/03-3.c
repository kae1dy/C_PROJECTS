#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

static const double L_PER = -100.0, H_PER = 100.0;

enum { ROUND = 10000 };

int
main(int argc, char *argv[]){
    double sum = 0, tmp;

    for (int i = 1; i < argc; ++i) {
        char *eptr = NULL;
        errno = 0;

        tmp = strtod(argv[i], &eptr);

        if (errno || *eptr || eptr == argv[i]) {
            fprintf(stderr, "INCORRECT INPUT.\n");
            return 1;
        }
        if (i == 1) {
            sum = tmp;
            continue;
        }
        if (L_PER <= tmp && tmp <= H_PER) {
            sum += tmp * sum / 100.0;

            sum = round(sum * ROUND) / ROUND;
        }
    }
    if (argc != 1) printf("%.4lf", sum);
    putchar('\n');
    return 0;
}