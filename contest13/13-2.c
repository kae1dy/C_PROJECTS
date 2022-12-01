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
#include <signal.h>

int
main(int argc, char *argv[])
{   
    int fd[2], pid;
    pipe(fd);

    if ((pid = fork()) == 0) {

        if ((pid = fork()) == 0) {
            int tmp;
            long long int sum = 0;
            close(fd[1]);
            while (read(fd[0], &tmp, sizeof(tmp)) > 0) sum += tmp;
            printf("%lld\n", sum); fflush(stdout);
            close(fd[0]);
            exit(0);
        } else {
            close(fd[0]);
            close(fd[1]);

            waitpid(pid, NULL, 0);
            exit(0);
        }
    } else {
        int tmp;
        close(fd[0]);
        while (scanf("%d", &tmp) == 1) {
            write(fd[1], &tmp, sizeof(tmp));
        }
        close(fd[1]);
        waitpid(pid, NULL, 0);
        exit(0);
        close(fd[1]);
    }
}