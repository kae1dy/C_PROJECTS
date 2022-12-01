#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

volatile int flag = 0;

void handlerINT(int s)
{   
    flag = 0;
}

void handlerQUIT(int s)
{   
    flag = 1;
}

int
main()
{
    sigaction(SIGINT, &(struct sigaction){ .sa_handler = handlerINT, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction){ .sa_handler = handlerQUIT, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid()); fflush(stdout);

    long long sum = 0;
    int tmp; 
    while (scanf("%d", &tmp) == 1) {
        if (flag) sum *= tmp;
        else sum += tmp;
        sum = (int) sum;
        printf("%lld\n", sum); fflush(stdout);
    }
    exit(EXIT_SUCCESS);
}