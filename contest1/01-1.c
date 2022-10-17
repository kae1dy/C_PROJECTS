#include <stdio.h>

int
main(int argc, char *argv[]){
    double sum = 0, tmp;

    for (int i = 1; i < argc; ++i) {

        if (sscanf(argv[i], "%lf", &tmp) != 1) {
            fprintf(stderr, "INCORRECT INPUT.\n");
            return 1;
        }
        if (i == 1) {
            sum = tmp;
            continue;
        }
        sum += tmp * sum / 100.0;
    }
    printf("%.4lf\n", sum);

    return 0;
}