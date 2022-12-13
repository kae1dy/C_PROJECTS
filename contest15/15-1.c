#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>

enum { ARG_CNT = 4, CHMOD = 0666 };

ushort getSPid(int val, int Nproc) {
    int mod = val % Nproc;
    return (((mod * mod) % Nproc) * ((mod * mod) % Nproc)) % Nproc;
}

int main(int argc, char *argv[])
{
    if (argc != ARG_CNT) exit(EXIT_FAILURE);

    errno = 0;
    char *eptr = NULL;
    long int Nproc = strtol(argv[1], &eptr, 10);
    if (errno || *eptr || argv[1] == eptr || (int) Nproc != Nproc) exit(EXIT_FAILURE);

    errno = 0;
    eptr = NULL;
    key_t key = strtol(argv[2], &eptr, 10);
    if (errno || *eptr || argv[2] == eptr) exit(EXIT_FAILURE);

    errno = 0;
    eptr = NULL;
    long int maxVal = strtol(argv[3], &eptr, 10);
    if (errno || *eptr || argv[3] == eptr || (int) maxVal != maxVal) exit(EXIT_FAILURE);

    int val = 0, sPid = 0, shmid, semid, pid;

    shmid = shmget(key, sizeof(val) + sizeof(sPid), IPC_CREAT | CHMOD);
    if (shmid < 0) exit(EXIT_FAILURE);

    semid = semget(key, Nproc, IPC_CREAT | CHMOD);
    if (semid < 0) exit(EXIT_FAILURE);

    int *shmaddr = shmat(shmid, NULL, 0);
    shmaddr[0] = val, shmaddr[1] = sPid;

    ushort *arrVals = calloc(Nproc, sizeof(*arrVals));
    if (!arrVals) exit(EXIT_FAILURE);

    arrVals[0] = 1;

    if (semctl(semid, 0, SETALL, arrVals) < 0) exit(EXIT_FAILURE);
    free(arrVals);

    for (int i = 1; i <= Nproc; ++i) {

        if ((pid = fork()) < 0) {
            semctl(semid, 0, IPC_RMID, 0);
            while (wait(NULL) > 0) {}

            shmdt(shmaddr);
            shmctl(shmid, IPC_RMID, NULL);

            exit(EXIT_FAILURE);
        }
        if (!pid) {

            struct sembuf sUp, sDown;

            sUp.sem_op = 1, sDown.sem_op = -1;
            sUp.sem_flg = 0, sDown.sem_flg = 0;
            sDown.sem_num = i - 1;

            while (1) {
                if (semop(semid, &sDown, 1) < 0) {
                    shmdt(shmaddr);
                    _exit(EXIT_SUCCESS);
                }

                val = shmaddr[0];
                if (val > maxVal) {
                    shmdt(shmaddr);
                    _exit(EXIT_SUCCESS);
                }
                printf("%d %d %d\n", i, val++, shmaddr[1]); fflush(stdout);

                ++shmaddr[0], shmaddr[1] = i;

                sUp.sem_num = getSPid(val, Nproc);
                semop(semid, &sUp, 1);
            }
        }
    }
    wait(NULL);

    semctl(semid, 0, IPC_RMID, 0);
    while (wait(NULL) > 0) {}

    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);

    exit(EXIT_SUCCESS);
}