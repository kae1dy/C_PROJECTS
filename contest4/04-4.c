#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    int fbin = open(argv[argc - 1], O_RDWR);

    // open file
    if (fbin == -1) {
        fprintf(stderr, "open file err\n");
        exit(1);
    }
    long long int tmp;

    if (read(fbin, &tmp, sizeof(tmp)) == sizeof(tmp)) {

        long long int min = tmp;
        off_t pos_min = 0;

        while (read(fbin, &tmp, sizeof(tmp)) == sizeof(tmp)) {
            
            if (min > tmp) {
                min = tmp;
                pos_min = lseek(fbin, 0, SEEK_CUR) - sizeof(tmp);
            }
        }
        // negative min number and write it
        if (min != LLONG_MIN) {

            min = -min;

            lseek(fbin, pos_min, SEEK_SET);

            if (write(fbin, &min, sizeof(min)) != sizeof(min)) {
                fprintf(stderr, "write err\n");
                exit(1);
            }
        }
    }
    // close file
    if (close(fbin) == -1) {
        fprintf(stderr, "close file err\n");
        exit(1);
    }
    return 0;
}