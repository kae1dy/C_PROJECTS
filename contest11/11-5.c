#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

const char ERR_STR[] = "-1\n";

int main(int argc, char *argv[])
{   
    int pid = fork();
    if (pid < 0) {
        printf("%s", ERR_STR), fflush(stdout); 
        _exit(EXIT_SUCCESS);
    }
    if (!pid) {
        while (1) {
            int status, num;
            if (scanf("%d", &num) != 1) _exit(EXIT_SUCCESS);

            pid = fork();
            if (pid < 0) {
                printf("%s", ERR_STR), fflush(stdout); 
                _exit(EXIT_FAILURE);
            }
            else if (pid > 0) {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    printf("%d\n", num), fflush(stdout);           
                    _exit(EXIT_SUCCESS);

                } else _exit(EXIT_FAILURE);
            }
        }
    }
    waitpid(pid, NULL, 0);
    _exit(EXIT_SUCCESS);
}