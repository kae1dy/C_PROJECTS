/* Многое взято из консультации Александра Владимировича Чернова.
   Прошу отнестись с пониманием :). ppp*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

enum { BUF_SIZE = 16, MAX_FD = 21 };

volatile sig_atomic_t sigReceived;

void
handler(int s) {
    sigReceived = s;
}

int
main (int argc, char *argv[])
{   
    int Nfd = argc - 1, nowInd = -1;
    int arrFD[MAX_FD] = {};
    long long sumFD[MAX_FD] = {};

    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handler }, NULL);

    sigset_t s1, s2;
    sigemptyset(&s1), sigemptyset(&s2);
    sigaddset(&s1, SIGTERM);

    for (int i = 0; i < Nfd; ++i) {
        sigaction(SIGRTMIN + i, &(struct sigaction) { .sa_handler = handler }, NULL);
        sigaddset(&s1, SIGRTMIN + i);

        arrFD[i] = open(argv[i + 1], O_RDONLY);
    }
    sigprocmask(SIG_BLOCK, &s1, NULL);

    printf("%d\n", getpid()); fflush(stdout);

    while (1) {
        if (nowInd == -1) {
            sigsuspend(&s2);

            if (sigReceived == SIGTERM) {
                break;
            }
            if (sigReceived >= SIGRTMIN) {
                nowInd = sigReceived - SIGRTMIN;
                if (arrFD[nowInd] < 0) nowInd = -1;
            }
            sigReceived = 0;
        } else {
            char buf[BUF_SIZE], *eptr;

            sigprocmask(SIG_UNBLOCK, &s1, NULL);
            ssize_t rbytes = read(arrFD[nowInd], buf, BUF_SIZE);
            sigprocmask(SIG_BLOCK, &s1, NULL);

            if (rbytes < 0 && errno == EINTR) {
                if (sigReceived == SIGTERM) {
                    break;
                }
                if (sigReceived >= SIGRTMIN) {
                    nowInd = sigReceived - SIGRTMIN;
                    if (arrFD[nowInd] < 0) nowInd = -1;
                }
                sigReceived = 0;

            } else if (!rbytes) {
                close(arrFD[nowInd]);
                arrFD[nowInd] = -1, nowInd = -1;

            } else sumFD[nowInd] += strtoll(buf, &eptr, 10);
        }
    }
    for (int i = 0; i < Nfd; ++i) {
        printf("%lld\n", sumFD[i]);
    }
    exit(EXIT_SUCCESS);
}