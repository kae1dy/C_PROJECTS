#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>


int
main(int argc, char *argv[]) {

    // open file
    int fbin = open(argv[argc - 2], O_RDWR);

    if (fbin == -1) {
        fprintf(stderr, "open file err\n");
        exit(1);
    }
    
    char *eptr = NULL;
    errno = 0;
    int N = strtod(argv[argc - 1], &eptr);

    if (errno || *eptr || eptr == argv[argc - 1]) {
        fprintf(stderr, "incorrect input.\n");
        exit(1);
    }
    double prev = 0, tmp;

    for (int i = 0; i < N; ++i) {

        if (read(fbin, &tmp, sizeof(tmp)) != sizeof(tmp)) {
            exit(0);
        }
        prev = tmp - prev;

        if (i == 0) continue;

        // modification file
        lseek(fbin, -((off_t) sizeof(tmp)), SEEK_CUR);

        if (write(fbin, &prev, sizeof(prev)) != sizeof(prev)) {
            fprintf(stderr, "write err\n");
            exit(1);
        }
    } 
    // close file
    if (close(fbin) == -1) {
        fprintf(stderr, "close file err\n");
        exit(0);
    }
    return 0;
}