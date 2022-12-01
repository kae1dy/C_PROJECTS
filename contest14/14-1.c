#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

volatile int count = 0;

void handler(int s)
{   
    if (count < 5) {
        printf("%d\n", count++); fflush(stdout);
    }
    else exit(EXIT_SUCCESS);
}

int
main()
{
    sigaction(SIGHUP, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid()); fflush(stdout);
    while (1) {
        pause();
    }
}