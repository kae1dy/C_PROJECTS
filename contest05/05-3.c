#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int
parse_rwx_permissions(const char *str)
{   
    const char str_ex[] = "rwxrwxrwx";

    int max_len = sizeof(str_ex) / sizeof(*str_ex) - 1;
    unsigned int answ = 0;

    if (str == NULL || (strlen(str) != max_len)) {
        return -1;
    }
    for (int i = max_len - 1; i >= 0; --i) {
                
        if (str[max_len - i - 1] == str_ex[max_len - i - 1]) {
            answ |= (1 << i);
        } else if (str[max_len - i - 1] != '-') return -1;

    }
    return answ;
}