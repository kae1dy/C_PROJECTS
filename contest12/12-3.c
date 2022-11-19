#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/mman.h>
#include <dlfcn.h>


int
main(int argc, char *argv[])
{   
    int pid, count = 0, status;

    for (int i = 1; i < argc; ++i) {

        if (argv[i][0] == 'p') {
            pid = fork();
            if (pid < 0) exit(1);

            if (pid == 0) {
                execlp(argv[i] + 1, argv[i] + 1, NULL);
                exit(1);
            }
        }
        else if (argv[i][0] == 's') {

            while (wait(&status) > 0) {
                if (WIFEXITED(status) && !WEXITSTATUS(status)) ++count;
            }

            pid = fork();
            if (pid < 0) exit(1);

            if (pid == 0) {
                execlp(argv[i] + 1, argv[i] + 1, NULL);
                exit(1);
            }
        }
    }
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) ++count;
    }
    printf("%d\n", count);

    exit(0);
}