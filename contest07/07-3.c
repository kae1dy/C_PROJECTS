#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

enum
{
    ARG_MAX = 3
};

static _Bool
isSame(struct stat stb1, struct stat stb2) {
    return (stb1.st_ino == stb2.st_ino && stb1.st_dev == stb2.st_dev);
}

int 
main(int argc, char *argv[])
{   
    if (argc != ARG_MAX) {
        fprintf(stderr, "incorrect num of arguments.\n");
        return 1;
    }
    char *dir1_path = argv[1];
    char *dir2_path = argv[2];

    DIR *dir1 = opendir(dir1_path);

    if (dir1 == NULL) {
        fprintf(stderr, "opendir error.\n");
        return 1;
    }
    char *file1_path = NULL, *file2_path = NULL;
    struct stat file1_st, file2_st;
    struct dirent *file1, *file2;

    unsigned long long sum = 0;

    while ((file1 = readdir(dir1)) != NULL) {

        asprintf(&file1_path, "%s/%s", dir1_path, file1->d_name);

        if (lstat(file1_path, &file1_st) != 0) {
            fprintf(stderr, "lstat error.\n");
            free(file1_path);
            closedir(dir1);
            return 1;
        }
        if (S_ISREG(file1_st.st_mode) && !S_ISLNK(file1_st.st_mode) && access(file1_path, W_OK) == 0) {

            DIR *dir2 = opendir(dir2_path);

            if (dir2 == NULL) {
                fprintf(stderr, "opendir error.\n");
                return 1;
            }

            while ((file2 = readdir(dir2)) != NULL) {

                if (strcmp(file2->d_name, "..") == 0) continue;
                if (strcmp(file2->d_name, ".") == 0) continue;

                asprintf(&file2_path, "%s/%s", dir2_path, file2->d_name);

                if (stat(file2_path, &file2_st) != 0) {
                    fprintf(stderr, "lstat error.\n");
                    free(file2_path);
                    return 1;
                }
                if (isSame(file1_st, file2_st) && strcmp(file1->d_name, file2->d_name) == 0) {
                    sum += file1_st.st_size;
                    free(file2_path);
                    break;
                }
                free(file2_path);
            }
            closedir(dir2);
        }
        free(file1_path);
    }
    closedir(dir1);

    printf("%llu\n", sum);
    return 0;
}