#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/msg.h>

enum { ARG_CNT = 6, CHMOD = 0666 };

typedef struct
{
    long mtype;
    long long x1;
    long long x2;
} Message;

int
main(int argc, char *argv[])
{
    if (argc != ARG_CNT) exit(EXIT_FAILURE);

    errno = 0;
    char *eptr = NULL;
    key_t key = strtol(argv[1], &eptr, 10);
    if (errno || *eptr || argv[1] == eptr) exit(EXIT_FAILURE);

    errno = 0;
    eptr = NULL;
    long int Nproc = strtol(argv[2], &eptr, 10);
    if (errno || *eptr || argv[2] == eptr || (int) Nproc != Nproc) exit(EXIT_FAILURE);

    long long x1, x2, maxVal;

    errno = 0;
    eptr = NULL;
    x1 = strtoll(argv[3], &eptr, 10);
    if (errno || *eptr || argv[3] == eptr) exit(EXIT_FAILURE);

    errno = 0;
    eptr = NULL;
    x2 = strtoll(argv[4], &eptr, 10);
    if (errno || *eptr || argv[4] == eptr) exit(EXIT_FAILURE);

    errno = 0;
    eptr = NULL;
    maxVal = strtoll(argv[5], &eptr, 10);
    if (errno || *eptr || argv[5] == eptr) exit(EXIT_FAILURE);

    int msgid, pid;
    msgid = msgget(key, IPC_CREAT | CHMOD);
    if (msgid < 0) exit(EXIT_FAILURE);

    Message mes = {.mtype = 1, .x1 = x1, .x2 = x2};
    for (int i = 1; i <= Nproc; ++i) {

        if ((pid = fork()) < 0) {
            msgctl(msgid, IPC_RMID, NULL);
            while(wait(NULL) > 0) {}

            exit(EXIT_FAILURE);
        }

        if (!pid) {     
            long long x3;
            while (1) {
                if (msgrcv(msgid, &mes, sizeof(mes) - sizeof(mes.mtype), i, 0) < 0) {
                    _exit(EXIT_SUCCESS);
                }
                x3 = mes.x1 + mes.x2;
                printf("%d %lld\n", i - 1, x3); fflush(stdout);

                if (x3 > maxVal) _exit(EXIT_SUCCESS);

                mes.mtype = x3 % Nproc + 1;
                mes.x1 = mes.x2, mes.x2 = x3;

                if (msgsnd(msgid, &mes, sizeof(mes) - sizeof(mes.mtype), 0) < 0) {
                    _exit(EXIT_FAILURE);
                }
            }
        }
    }
    msgsnd(msgid, &mes, sizeof(mes) - sizeof(mes.mtype), 0);
    wait(NULL);

    msgctl(msgid, IPC_RMID, NULL);
    while(wait(NULL) > 0) {}

    exit(EXIT_SUCCESS);
}