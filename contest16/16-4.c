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
    unsigned tmp, size = 0, max = 0;

    while (scanf("%u", &tmp) == 1) {
        unsigned mask = 1u << 31;
        while (tmp % mask != 0) mask >>= 1;
        
        size += (mask - size % mask) % mask + tmp;
        if (mask > max) max = mask;
    }
    size += (max - size % max) % max;
    printf("%u %u\n", (size) ? size : 1, (size) ? max : 1);
    exit(0);
}