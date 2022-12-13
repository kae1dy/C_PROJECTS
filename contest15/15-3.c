#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>

enum { ARG_CNT = 2, CHMOD = 0666, KEY = 5 };

int main(int argc, char *argv[])
{
    if (argc != ARG_CNT) exit(EXIT_FAILURE);

    setbuf(stdin, NULL);

    errno = 0;
    char *eptr = NULL;
    long int Nproc = strtol(argv[1], &eptr, 10);
    if (errno || *eptr || argv[1] == eptr || (int) Nproc != Nproc) exit(EXIT_FAILURE);

    int semid, pid;
    semid = semget(KEY, Nproc, IPC_CREAT | CHMOD);
    if (semid < 0) exit(EXIT_FAILURE);

    ushort *arrVals = calloc(Nproc, sizeof(*arrVals));
    if (!arrVals) exit(EXIT_FAILURE);
    arrVals[0] = 1;

    if (semctl(semid, 0, SETALL, arrVals) < 0) exit(EXIT_FAILURE);

    for (int i = 0; i < Nproc; ++i) {

        if ((pid = fork()) < 0) {
            
            semctl(semid, 0, IPC_RMID, 0);
            while (wait(NULL) > 0) {}

            free(arrVals);
            exit(EXIT_FAILURE);
        }

        if (!pid) {
            struct sembuf sUp, sDown;
            int val;

            sUp.sem_op = 1, sDown.sem_op = -1;
            sUp.sem_flg = 0, sDown.sem_flg = 0;
            sDown.sem_num = i;

            while (1) {
                if (semop(semid, &sDown, 1) < 0) {
                    _exit(EXIT_SUCCESS);
                }

                if (scanf("%d", &val) != 1) {
                    _exit(EXIT_SUCCESS);
                }
                printf("%d %d\n", i, val); fflush(stdout);

                sUp.sem_num = (val >= 0) ? val % Nproc : (Nproc + val % Nproc) % Nproc;
                if (semop(semid, &sUp, 1) < 0) {
                    _exit(EXIT_FAILURE);
                }
            }
        }
    }
    wait(NULL);

    semctl(semid, 0, IPC_RMID, 0);
    while (wait(NULL) > 0) {}

    free(arrVals);
    exit(EXIT_SUCCESS);
}