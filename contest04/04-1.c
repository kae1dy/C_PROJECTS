#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>

enum
{
    MASK_BYTE = 0xFF,
    SIZE = 4
};

static const mode_t CHMOD = 0600;

int
main(int argc, char *argv[]) {
    uint32_t num;

    int f = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, CHMOD);

    if (f == -1) {
        fprintf(stderr, "open file err\n");
        exit(1);
    }
    while (scanf("%u", &num) == 1) {
        uint8_t buf[SIZE] = {};

        for (int i = SIZE - 1; i >= 0; --i) {

            if (i % 2) {
                buf[i] = (uint8_t) (num & MASK_BYTE);
                num >>= CHAR_BIT;
            } else {
                buf[i] = (uint8_t) (num & (MASK_BYTE >> 4));
                num >>= (CHAR_BIT / 2);
            }
        }
        if (write(f, buf, sizeof(buf)) != sizeof(buf)) {
            fprintf(stderr, "write err\n");
            exit(1);
        }
    }
    if (close(f) == -1) {
        fprintf(stderr, "close file err\n");
        exit(1);
    }
    return 0;
}