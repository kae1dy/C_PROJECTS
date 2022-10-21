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

    if (buf == NULL) return;

    size_t size = strlen(buf);

    buf[size] = '/';

    int counter = 0, i, j;
    char flag = 0;

    for (i = size - 1; i > 0; --i) {

        if (buf[i - 1] == '/' && buf[i] == '.' && buf[i + 1] == '/') {
            buf[i] = 0, buf[i + 1] = 0;

        } else if (i >= 2 && buf[i - 2] == '/' && buf[i - 1] == '.' &&
                buf[i] == '.' && buf[i + 1] == '/') {
            buf[i - 1] = 0, buf[i] = 0, buf[i + 1] = 0;
            if (flag) counter = 0, flag = 0;
            --i, ++counter;

        if (flag) {
            if (buf[i] == '/') --counter;
            if (counter == 0) flag = 0;
            buf[i] = 0;

        } if (buf[i] == '/') continue;

        } else if (counter > 0) {
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
    char s[] = "/aa/../../../../../../aa/../aa/";
    normalize_path(s);
    printf("%s", s);
}