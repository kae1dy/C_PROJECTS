#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

enum
{
    ARG_MAX = 2,
    BUF_SIZE = 8
};

#define ABS(x) (((x) >= 0) ? (x) : -(x))

int main(int argc, char *argv[])
{   
    if (argc != ARG_MAX) {
        fprintf(stderr, "incorrect number of arg.\n");
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open file error.\n");
        return 1;
    }
    unsigned char mask = 0;
    int tmp, bytes;

    struct stat st;
    if (fstat(fd, &st) != 0) return 1;

    while (scanf("%d", &tmp) == 1) {

        if ((st.st_size < ABS(tmp / __CHAR_BIT__)) || tmp == 0) continue;

        if (tmp > 0) {
            tmp -= 1;
            bytes = tmp / __CHAR_BIT__;

            errno = 0;
            if (lseek(fd, (off_t) bytes, SEEK_SET) && errno) return 1;
            if (read(fd, &mask, sizeof(mask)) != 1) return 1;

            mask |= 1 << (tmp % __CHAR_BIT__);

        } else {
            tmp = -tmp - 1;
            bytes = tmp / __CHAR_BIT__;

            errno = 0;
            if (lseek(fd, (off_t) bytes, SEEK_SET) && errno) return 1;
            if (read(fd, &mask, sizeof(mask)) != 1) return 1;

            mask &= ~(1 << (tmp % __CHAR_BIT__));
        }
        errno = 0;
        if (lseek(fd, -1, SEEK_CUR) == -1 && errno) return 1;
        if (write(fd, &mask, sizeof(mask)) != 1) return 1;
    }
    if (close(fd) == -1) {
        fprintf(stderr, "close file error.\n");
        return 1;
    }
    return 0;
}