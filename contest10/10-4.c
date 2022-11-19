#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/mman.h>
#include <dlfcn.h>

enum
{
    SIZE_STACK = 64,
    ARG_MIN = 4,
};

struct Stack 
{ 
    char buf[SIZE_STACK];
    int size;
};

struct Stack Stack = {{}, 0};

int
DoubleToStack(double num)
{
    if ((Stack.size += sizeof(num)) > SIZE_STACK) return -1;
    *(double *)(Stack.buf + Stack.size - sizeof(num)) = num;

    return 0;
}

int
IntToStack(int num)
{

    if ((Stack.size += sizeof(num)) > SIZE_STACK) return -1;
    *(int *)(Stack.buf + Stack.size - sizeof(num)) = num;

    return 0;
}
int
StrToStack(char *str)
{
    if ((Stack.size += sizeof(str)) > SIZE_STACK) return -1;
    *(char **)(Stack.buf + Stack.size - sizeof(str)) = str;

    return 0;
}

int
main(int argc, char *argv[])
{   
    int N = argc - ARG_MIN + 1;

    if (argc < ARG_MIN || N != strlen(argv[3])) {
        fprintf(stderr, "Incorrect number of args.\n");
        exit(1);
    }

    for (int i = 1; i < N; ++i) {
        char symbol = argv[3][i], *eptr = NULL;
        errno = 0;

        if (symbol == 'd') {
            double tmp = strtod(argv[i + 3], &eptr);
            if (errno || *eptr || eptr == argv[i + 3]) {
                fprintf(stderr, "Incorrect argument.\n");
                exit(1);
            }
            if (DoubleToStack(tmp) < 0) exit(1);
        }
        else if (symbol == 'i') { 
            long int tmp = strtol(argv[i + 3], &eptr, 10);
            if (errno || *eptr || eptr == argv[i + 3] || (int) tmp != tmp) {
                fprintf(stderr, "Incorrect argument.\n");
                exit(1);
            }
            if (IntToStack((int) tmp) < 0) exit(1);
        }
        else if (symbol == 's') { 
            if (StrToStack(argv[i + 3]) < 0) exit(1);
        }
    }
    void *dll = dlopen(argv[1], RTLD_LAZY);
    if (dll == NULL) {
        fprintf(stderr, "dlopen error: %s.\n", dlerror());
        exit(1);
    }

    char symbol = argv[3][0];

    if (symbol == 'd') {
        double (*ptrFunc) (struct Stack) = dlsym(dll, argv[2]);
        if (ptrFunc == NULL) exit(1);
        printf("%.10g\n", (*ptrFunc)(Stack));

    } else if (symbol == 'i') {
        int (*ptrFunc)(struct Stack) = dlsym(dll, argv[2]);
        if (ptrFunc == NULL) exit(1);
        printf("%d\n", (*ptrFunc)(Stack));

    } else if (symbol == 's') {
        char * (*ptrFunc) (struct Stack) = dlsym(dll, argv[2]);
        if (ptrFunc == NULL) exit(1);
        printf("%s\n", (*ptrFunc)(Stack));

    } else if (symbol == 'v') {
        void (*ptrFunc) (struct Stack) = dlsym(dll, argv[2]);
        if (ptrFunc == NULL) exit(1);
        (*ptrFunc)(Stack);
    }
    
    if (dlclose(dll) < 0) {
        fprintf(stderr, "dlclose error.\n");
        exit(1);
    }
    exit(0);
}
