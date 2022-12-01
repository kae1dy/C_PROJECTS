#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <sys/signalfd.h>

enum { ARG = 2, PID1 = 1, PID2 = 2 };

void pinPong(FILE *rfile, FILE  *wfile, long N, int mode)
{
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1);
    int sfd = signalfd(-1, &ss, 0);

    while (1) {
        struct signalfd_siginfo sinfo;
        read(sfd, &sinfo, sizeof(sinfo));

        int temp;
        if (fscanf(rfile, "%d", &temp) != 1) {
            kill(sinfo.ssi_pid, SIGQUIT);
            exit(0);
        }
        printf("%d %d\n", mode, temp++); fflush(stdout);
        if (temp >= N) {
            kill(sinfo.ssi_pid, SIGQUIT);
            exit(0);
        }
        fprintf(wfile, "%d\n", temp); fflush(wfile); 
        kill(sinfo.ssi_pid, SIGUSR1);
    }
}

int main(int argc, char *argv[])
{   
    if (argc != ARG) _exit(EXIT_SUCCESS);

    char *eptr = NULL;
    errno = 0;
    long int N = strtol(argv[argc - 1], &eptr, 10);
    if (errno || *eptr || argv[argc - 1] == eptr || (int) N != N) _exit(1);

    if (N <= 1) {
        printf("Done\n"); fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    int fd[2], pid1, pid2;
    if (pipe(fd) < 0) _exit(EXIT_FAILURE);

    FILE *rfile = fdopen(fd[0], "r");
    FILE *wfile = fdopen(fd[1], "w");

    if (!rfile || !wfile) _exit(EXIT_FAILURE);

    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss, NULL);

    if ((pid1 = fork()) < 0) _exit(EXIT_FAILURE);

    if (pid1 == 0) {
        pinPong(rfile, wfile, N, PID1);
        _exit(0);
    }
    if ((pid2 = fork()) < 0) _exit(EXIT_FAILURE);

    if (pid2 == 0) {
        fprintf(wfile, "1\n"); fflush(wfile); 
        kill(pid1, SIGUSR1);
        pinPong(rfile, wfile, N, PID2);
        _exit(0);
    }
    fclose(wfile), fclose(rfile);
    while (wait(NULL) > 0) {}
    printf("Done\n"); fflush(stdout);
    _exit(EXIT_SUCCESS);
}