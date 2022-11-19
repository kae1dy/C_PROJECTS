#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/mman.h>
#include <dlfcn.h>

int launch(char *name) {

    int pid = fork();
    if (pid < 0) return 1;

    else if (!pid) {
        execlp(name, name, NULL);
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        return 1;
    } else return 0;
}

int
main(int argc, char *argv[])
{   
    if ((launch(argv[1]) || launch(argv[2])) && launch(argv[3])) {
        exit(0);
    }
    exit(1);
}