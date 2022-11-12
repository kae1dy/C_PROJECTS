#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

enum
{
    ARG_MAX = 2
};

int
main(int argc, char *argv[])
{   
    if (argc != ARG_MAX) {
        fprintf(stderr, "incorrect number of arg.\n");
        return 1;
    }
    char *cg_path = argv[argc - 1];
    DIR *cg = opendir(cg_path);

    if (cg == NULL) {
        fprintf(stderr, "opendir error.\n");
        return 1;
    }
    struct dirent *file;
    struct stat buf;

    int count = 0;

    while ((file = readdir(cg)) != NULL) {

        size_t len_cg = strlen(cg_path);
        size_t len_name = strlen(file->d_name);

        if (len_name < 4) continue;

        char *file_path = calloc(len_cg + len_name + 2, sizeof(*file_path));

        memcpy(file_path, cg_path, len_cg * sizeof(*cg_path));
        file_path[len_cg++] = '/';
        memcpy(file_path + len_cg, file->d_name, len_name * sizeof(*file->d_name));

        stat(file_path, &buf);

        if (S_ISREG(buf.st_mode) && access(file_path, X_OK) == 0) {
            
            if (file->d_name[len_name - 1] == 'e' &&
                file->d_name[len_name - 2] == 'x' &&
                file->d_name[len_name - 3] == 'e' && 
                file->d_name[len_name - 4] == '.') {
                
                ++count;
            }
        }
        free(file_path);
    }
    printf("%d\n", count);

    if (closedir(cg) == -1) {
        fprintf(stderr, "closedir error.\n");
        return 1;
    }
    return 0;
}