#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

enum 
{ 
    SIZE = 8,
    PROC = 3
 };

int main(int argc, char *argv[])
{
    for (int i = 1; i <= PROC; ++i ) {
        if (!fork()) {
            char buf[SIZE] = {}, *eptr = NULL;

            if (read(0, buf, SIZE) != SIZE) {
                exit(1);
            }
            errno = 0;
            long int tmp = strtol(buf, &eptr, 10);
            if (errno || eptr == buf || (int) tmp != tmp) {
                exit(1);
            }
            printf("%d %ld\n", i, tmp * tmp);
            exit(0);
        }
    }
    while (wait(NULL) > 0) {};
    exit(0);
}