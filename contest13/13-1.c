#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

int
main(int argc, char *argv[])
{   
    int fd[2], pid;
    pipe(fd);

    if ((pid = fork()) == 0) {
        if ((pid = fork()) == 0) {
            close(fd[0]);
            time_t tmp;
            struct tm now = {.tm_isdst = -1};
            time(&tmp);
            write(fd[1], &tmp, sizeof(tmp));
            write(fd[1], &tmp, sizeof(tmp));
            close(fd[1]);

            localtime_r(&tmp, &now);
            printf("D:%02d\n", now.tm_mday);
            exit(0);
        } else {
            close(fd[1]);
            waitpid(pid, NULL, 0);
            time_t tmp;
            struct tm now = {.tm_isdst = -1};

            read(fd[0], &tmp, sizeof(tmp));
            close(fd[0]);

            localtime_r(&tmp, &now);
            printf("M:%02d\n", now.tm_mon + 1);
            exit(0);
        }
    } else {
        close(fd[1]);
        waitpid(pid, NULL, 0);
        time_t tmp;
        struct tm now = {.tm_isdst = -1};

        read(fd[0], &tmp, sizeof(tmp));

        close(fd[0]);

        localtime_r(&tmp, &now);
        printf("Y:%04d\n", now.tm_year + 1900);
        exit(0);
    }
}