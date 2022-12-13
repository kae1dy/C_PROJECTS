#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>

enum { MAX = 3, BASE = 10, DIGMAX = 12 };

int itoa(int N, char *buf, int base) {
    int count = 0;
    char digit;
    do { 
        digit = N % base  + '0';
        buf[count++] = digit;
        N /= base;
    } while (N);
    
    for (int i = 0; i < count / 2; ++i) {
        digit = buf[i];
        buf[i] = buf[count - i - 1];
        buf[count - i - 1] = digit;
    }
    buf[count] = '\n';
    return count + 1;
}

volatile sig_atomic_t primeLast;

void
handlerINT(int s) {
    static int count = 0;
    if (++count > MAX) _exit(EXIT_SUCCESS);

    char buf[DIGMAX] = {};
    int size = itoa(primeLast, buf, BASE);

    write(STDOUT_FILENO, buf, size), fflush(stdout);

}
void
handlerTERM(int s) {
    _exit(EXIT_SUCCESS);
}

int
isPrime(int N) {

    if (N <= 1) return 0;
    int sqN = (int) sqrt(N);
    
    for (int i = 2;  i <= sqN; ++i) {
        if (N % i == 0) return 0;
    }
    return 1;
}

int
main(void) {

    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handlerINT, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handlerTERM, .sa_flags = SA_RESTART }, NULL);
    
    int L, R;
    printf("%d\n", getpid()), fflush(stdout);

    if (scanf("%d%d", &L, &R) != 2) _exit(EXIT_FAILURE);

    for (; L < R; ++L) {
        if (isPrime(L)) primeLast = L;
    }
    printf("-1\n"), fflush(stdout);
    _exit(EXIT_SUCCESS);
}