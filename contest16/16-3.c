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


int
main(int argc, char *argv[])
{
    unsigned N, tmp;
    scanf("%u", &N);

    unsigned *addrFAT = calloc(N, sizeof(*addrFAT));
    if (!addrFAT) exit(EXIT_FAILURE);

    for (int i = 2; i < N - 2; ++i) {
        scanf("%u", &addrFAT[i]);
    }

    while (scanf("%u", &tmp) == 1) {
        unsigned count = 0;

        while (tmp >= 2 && tmp < N - 2) {
            ++count;
            tmp = addrFAT[tmp];
        }
        if (tmp == N - 2) ++count; 
        printf("%u\n", count);
    }
    free(addrFAT);
    exit(EXIT_SUCCESS);
}