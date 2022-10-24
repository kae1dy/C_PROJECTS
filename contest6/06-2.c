#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void 
normalize_path(char *buf) { 

    size_t size = strlen(buf);
    if (buf == NULL || (size = strlen(buf)) == 0) return;

    buf[size] = '/';

    int i, j, counter = 0;
    char flag = 0;

    for (i = size - 1; i > 0; --i) {

        if (buf[i - 1] == '/' && buf[i] == '.' && buf[i + 1] == '/') {
            buf[i] = 0, buf[i + 1] = 0;
            continue;
        }         
        if (i >= 2 && buf[i - 2] == '/' && buf[i - 1] == '.' &&
                buf[i] == '.' && buf[i + 1] == '/') {
            buf[i - 1] = 0, buf[i] = 0, buf[i + 1] = 0;

            if (flag) counter = 0, flag = 0;

            --i, ++counter;
            continue;
        }
        if (flag) {
            if (buf[i] == '/') --counter, flag = 0;
            else buf[i] = 0;
        }
        if (buf[i] == '/') continue;

        if (counter > 0) {
            buf[i] = 0, buf[i + 1] = 0;
            flag = 1;
        }
    }
    for (i = 1, j = 1; i < size; ++i) {
        if (buf[i] != 0) {
            buf[j++] = buf[i];
        }
    }
    if (j != 1 && buf[j - 1] == '/') buf[j - 1] = 0;
    else buf[j] = 0;
}

int
main(int argc, char *argv[])
{   
    char s[] = "/./././../../ab../..c/";
    normalize_path(s);
    printf("%s", s);
}