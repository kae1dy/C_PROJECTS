#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>


enum { ARG_MAX = 2 };

int main(int argc, char *argv[])
{   
    if (argc != ARG_MAX) _exit(EXIT_SUCCESS);

    int pid1, pid2;

    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1);
    sigaddset(&ss, SIGUSR2);
    sigaddset(&ss, SIGIO);

    sigprocmask(SIG_BLOCK, &ss, NULL);

    if ((pid1 = fork()) < 0) _exit(EXIT_FAILURE);

    if (pid1 == 0) {
        siginfo_t tmp;
        unsigned char mask = 1, buf = 0;
        
        while (1) {
            sigwaitinfo(&ss, &tmp);

            if (tmp.si_signo == SIGUSR2) {
                buf |= mask;
            } else if (tmp.si_signo == SIGIO) {
                _exit(0);
            }
            mask <<= 1;
            if (!mask) {
                printf("%c", buf), fflush(stdout);
                mask = 1, buf = 0;
            }
            kill(tmp.si_pid, SIGALRM);
        }
    }
    if ((pid2 = fork()) < 0) _exit(EXIT_FAILURE);

    if (pid2 == 0) {

        int rfd = open(argv[1], O_RDONLY);
        if (rfd < 0) {
            kill(pid1, SIGIO);
            _exit(1);
        }
        sigset_t ss;
        sigemptyset(&ss);
        sigaddset(&ss, SIGALRM);
        sigprocmask(SIG_BLOCK, &ss, NULL);

        while (1) {
            unsigned char buf = 0;
            if (read(rfd, &buf, sizeof(buf)) <= 0) {
                kill(pid1, SIGIO);
                close(rfd);
                _exit(0);
            }
            for (int i = 0; i < __CHAR_BIT__; ++i) {
                if (buf & 1u) kill(pid1, SIGUSR2);
                else kill(pid1, SIGUSR1);
                buf >>= 1;
                sigwaitinfo(&ss, NULL);
            }
        }
    }
    while (wait(NULL) > 0) {}
    _exit(EXIT_SUCCESS);
}