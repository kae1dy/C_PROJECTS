#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

pid_t *childPID = NULL;
volatile sig_atomic_t count = 0;

void handler(int s) {
    for (int i = 0; i < count; ++i) {
        kill(childPID[i], SIGKILL);
    }
    while (wait(NULL) > 0) {}
    free(childPID);
    _exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{   
    int fd[2] = { 0, 1 }, ppid = getpid();
    childPID = calloc(argc, sizeof(*childPID));
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    for (int i = 1; i < argc; ++i) {
        int tmpfd[2] = { 0, 1 };
        if (i != argc - 1) {
            if (pipe2(tmpfd, O_CLOEXEC) < 0) {
                kill(ppid, SIGUSR1);
                _exit(EXIT_FAILURE);
            }
        }
        int pid = fork();
        if (pid < 0) {
            kill(ppid, SIGUSR1);
            _exit(EXIT_FAILURE);
        } 
        if (!pid) {
            free(childPID);
            if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(tmpfd[1], STDOUT_FILENO) < 0) {
                kill(ppid, SIGUSR1);
                _exit(EXIT_FAILURE);
            }
            execlp(argv[i], argv[i], NULL);
            kill(ppid, SIGUSR1);

            _exit(EXIT_FAILURE);
        }
        childPID[count++] = pid;
        if (tmpfd[1] != 1) close(tmpfd[1]);
        if (fd[0] != 0) close(fd[0]);

        fd[0] = tmpfd[0];
    }
    while (wait(NULL) > 0) {}
    free(childPID);
    _exit(EXIT_SUCCESS);
}