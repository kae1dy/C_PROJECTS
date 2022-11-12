#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/mman.h>

enum 
{ 
    A = 1103515245,
    C = 12345,
    M = 1<<31
};

typedef struct RandomOperations RandomOperations;

typedef struct RandomGenerator
{
    RandomOperations *ops;
    long int value;
} RandomGenerator;

struct RandomOperations
{
    int(*next) (RandomGenerator *);
    void(*destroy) (RandomGenerator *);
};

void destroy(RandomGenerator *head) {
    free(head);
}

int next(RandomGenerator *head) {
    if (head == NULL) return 0;
    head->value = (A * head->value + C) % M; 
    return head->value;
}

RandomOperations opsRandom = {&next, &destroy};

RandomGenerator
*random_create(int seed) 
{
    RandomGenerator *head = calloc(1, sizeof(*head));
    if (head != NULL) {
        head->value = seed;
        head->ops = &opsRandom;
    }
    return head;
}