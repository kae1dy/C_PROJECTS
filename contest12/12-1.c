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


int
main(int argc, char *argv[])
{   
    int pid = fork();
    
    if (pid < 0) exit(0);

    else if (!pid) {

        int fd = open(argv[2], O_RDONLY);
        if (fd < 0) {
            _exit(42);
        }
        dup2(fd, STDIN_FILENO);
        if (close(fd) < 0) _exit(42);

        fd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (fd < 0) {
            _exit(42);
        }
        dup2(fd, STDOUT_FILENO);
        if (close(fd) < 0) _exit(42);

        fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fd < 0) {
            _exit(42);
        }
        dup2(fd, STDERR_FILENO);
        if (close(fd) < 0) _exit(42);

        execlp(argv[1], argv[1], NULL);
        exit(42);
    }
    int status;
    wait(&status);
    printf("%d\n", status);

    exit(0);
}