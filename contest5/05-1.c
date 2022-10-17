#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

enum
{
    KIB = 1 << 10
};

int
main(int argc, char *argv[])
{
    unsigned long long int sum = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat stb;

        if (lstat(argv[i], &stb) >= 0 && S_ISREG(stb.st_mode) &&
                !S_ISLNK(stb.st_mode) && !(stb.st_size % KIB) && stb.st_nlink == 1) {
            sum += stb.st_size;
        }
    }
    printf("%lld\n", sum);
}

