#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <sys/wait.h>

#define MIN(x,y) ((x) > (y) ? (y) : (x))

int
main(int argc, char *argv[])
{
    int count = 0;
    long N = strtol(argv[1], NULL, 10);
    
    for (int i = 2; i < MIN(N, argc) + 2; ++i) {
        int pid = fork();
        if (pid < 0) exit(EXIT_FAILURE);
        if (!pid) {
            FILE *f = fopen(argv[i], "r");
            if (!f) exit(EXIT_FAILURE);

            char bin[PATH_MAX];
            fscanf(f, "%s\n", bin);
            fclose(f);

            execlp(bin, bin, NULL);
            exit(EXIT_FAILURE);
        }
    }
    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) ++count;
    }

    for (int i = N + 2; i < argc; ++i) {
        int pid = fork();
        if (pid < 0) exit(EXIT_FAILURE);
        if (!pid) {
            FILE *f = fopen(argv[i], "r");
            if (!f) exit(EXIT_FAILURE);

            char bin[PATH_MAX];
            fscanf(f, "%s\n", bin);
            fclose(f);

            execlp(bin, bin, NULL);
            exit(EXIT_FAILURE);
        }
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) ++count;
    }
    printf("%d", count);
    exit(EXIT_SUCCESS);
}