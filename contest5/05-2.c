#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

enum
{
    LIMIT = 0777,
};

const char str[] = "rwxrwxrwx";

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {

        char *eptr = NULL;
        errno = 0;
        long val = strtol(argv[i], &eptr, 8);

        if (errno || *eptr || eptr == argv[i] || val > LIMIT) {
            fprintf(stderr, "incorrect input\n");
            return 1;
        } else {
            for (int j = 8; j >= 0; --j) {
                if ((1 << j) & val) {
                    printf("%c", str[8 - j]);
                } else {
                    printf("-");
                }
            }
        }
        putchar('\n');
    }

}