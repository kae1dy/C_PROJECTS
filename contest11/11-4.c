#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int N, pid;
    scanf("%d", &N);

    printf("1 "); fflush(stdout);

    for (int i = 2; i <= N; ++i) {

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork error.\n");
            exit(1);
        }
        if (!pid) {
            if (i != N) printf("%d ", i);
            else printf("%d", i);
            fflush(stdout);
        } else {
            waitpid(pid, NULL, 0);
            exit(0);
        }
    }
    wait(NULL);
    putchar('\n');
    exit(0);
}