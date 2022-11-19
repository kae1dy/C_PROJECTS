#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>

#define errorExit(errorCode) fprintf(stderr, "create script error.\n"), exit(errorCode);

enum 
{ 
    CHMOD = 0700,
    ARG_MIN = 2
};

const char header[] = "#! /usr/bin/python3\n"
                        "import sys\n"
                        "from os import remove\n"
                        "from sys import argv\n"
                        "sys.set_int_max_str_digits(1000000)\n"
                        "print(";

const char footer[] = ")\nremove(argv[0])\n";
const char multiply = '*';

int
getPath(char *dist, size_t size, char *def) {
    char *dirpath;
    struct timeval timenow;

    if (gettimeofday(&timenow, NULL) < 0) return -1;
     
    dirpath = (getenv("XDG_RUNTIME_DIR") != NULL) ? getenv("XDG_RUNTIME_DIR") : getenv("TMPDIR");
    if (!dirpath) dirpath = def;
    if (snprintf(dist, size, "%s/runme%ld", dirpath, timenow.tv_usec) < 0) return -1;
    return 0;
}

int
main(int argc, char *argv[])
{   
    if (argc < ARG_MIN) exit(EXIT_SUCCESS);

    char path_script[PATH_MAX] = {};
    if (getPath(path_script, PATH_MAX, "/tmp/") < 0) errorExit(EXIT_FAILURE);

    int fd_script = open(path_script, O_RDWR | O_CREAT | O_TRUNC, CHMOD);
    if (fd_script < 0) errorExit(EXIT_FAILURE);

    if (write(fd_script, header, sizeof(header) - 1) != sizeof(header) - 1) errorExit(EXIT_FAILURE);

    for (int i = 1; i < argc; ++i) {

        if (i != 1 && write(fd_script, &multiply, sizeof(multiply)) != sizeof(multiply)) errorExit(EXIT_FAILURE);

        size_t len = strlen(argv[i]);
        if (write(fd_script, argv[i], len) != len) errorExit(EXIT_FAILURE);
    }
    if ((write(fd_script, footer, strlen(footer) - 1)) != strlen(footer) - 1) errorExit(EXIT_FAILURE);
    if (close(fd_script) < 0) errorExit(EXIT_FAILURE);

    execl(path_script, path_script, NULL);
    exit(EXIT_FAILURE);
}