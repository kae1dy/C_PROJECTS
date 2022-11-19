#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int pid1, pid2;

    if ((pid1 = fork()) < 0) {
        fprintf(stderr, "fork error.\n");
        _exit(1);
    }

    if (!pid1) {
        if ((pid2 = fork()) < 0) {
            fprintf(stderr, "fork error.\n");
            _exit(1);
        }
        if (!pid2) {
            printf("3 ");
            _exit(0);
        }
        wait(NULL);
        printf("2 "), fflush(stdout);
        _exit(0);
    }
    wait(NULL);
    printf("1 "), fflush(stdout);
    exit(0);
}