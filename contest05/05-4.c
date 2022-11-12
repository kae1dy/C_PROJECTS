#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int
comp_paths(const void *path1, const void *path2)
{
    char *path1_tmp = *(char **) path1;
    char *path2_tmp = *(char **) path2;
    return strcmp(path1_tmp, path2_tmp);
}

int
main(int argc, char *argv[])
{   
    char **arr_str = calloc(argc - 1, sizeof(*arr_str));
    int counter = 0;

    for (int i = 1; i < argc; ++i) {
        _Bool flag = 1;

        struct stat stb1, stb2;
        if (stat(argv[i], &stb1) >= 0) {
            
            for (int j = 0; j < counter; ++j)
            {
                stat(arr_str[j], &stb2);

                if (stb1.st_ino == stb2.st_ino) {
                    
                    if (strcmp(argv[i], arr_str[j]) == 1) {
                        arr_str[j] = argv[i];
                    }
                    flag = 0;
                    break;
                }
            }
            if (flag) arr_str[counter++] = argv[i];
        }
    }
    qsort(arr_str, counter, sizeof(*arr_str), comp_paths);

    for (int i = 0; i < counter; i++) {
        puts(arr_str[i]);
    }
    return 0;
}
