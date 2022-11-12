#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

int
isBigEndian(void)
{
    int num = 1;
    return (*(char *) &num != 1);
}

void
swapOrder(unsigned char *buf)
{   
    for (size_t i = 0; i < __SIZEOF_LONG_LONG__ / 2; ++i)
    {   
        buf[i] ^= buf[__SIZEOF_LONG_LONG__ - i - 1];
        buf[__SIZEOF_LONG_LONG__ - i - 1] ^= buf[i];
        buf[i] ^= buf[__SIZEOF_LONG_LONG__ - i - 1];
    }
    return;
}

int
main(int argc, char *argv[])
{
    int FLAG_BIG_END = isBigEndian();

    int fbin = open(argv[argc - 1], O_RDWR);

    // open file
    if (fbin == -1) {
        fprintf(stderr, "open file err\n");
        exit(1);
    }
    union {
        long long int tmp;
        unsigned char buf[__SIZEOF_LONG_LONG__];
    } num;

    if (read(fbin, &num, sizeof(num)) == sizeof(num)) {

        if (FLAG_BIG_END) swapOrder(num.buf);
        long long int min = num.tmp;
        off_t pos_min = 0;

        while (read(fbin, &num, sizeof(num)) == sizeof(num)) {
            
            if (FLAG_BIG_END) swapOrder(num.buf);

            if (min > num.tmp) {
                min = num.tmp;
                pos_min = lseek(fbin, 0, SEEK_CUR) - sizeof(num);
            }
        }
        // negative min number and write it
        if (min != LLONG_MIN) {

            num.tmp = -min;

            if (FLAG_BIG_END) swapOrder(num.buf);
            lseek(fbin, pos_min, SEEK_SET);

            if (write(fbin, &num, sizeof(num)) != sizeof(num)) {
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