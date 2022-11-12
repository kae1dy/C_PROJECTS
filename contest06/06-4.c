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
    ARGMAX = 3,
    DEATH = 4
};

// ./main path z
void
search_files(const char *path, const char *rel_path, off_t size_file, int depth)
{   
    char *tmp_path;
    if (rel_path == NULL) {
        asprintf(&tmp_path, "%s", path);
    }
    else {
        asprintf(&tmp_path, "%s/%s", path, rel_path);
    }

    DIR *curr = opendir(tmp_path);
    if (curr == NULL) return;

    char **dirs = NULL;
    int dir_num = 0, size = 0;

    char *curr_path = NULL;
    struct dirent *curr_d;

    while ((curr_d = readdir(curr)) != NULL) {
        struct stat stb;

        free(curr_path);

        asprintf(&curr_path, "%s/%s", tmp_path, curr_d->d_name);

        if (lstat(curr_path, &stb) != 0) continue;

        if (S_ISDIR(stb.st_mode)) {
            if (strcmp(curr_d->d_name, ".") == 0) continue;
            if (strcmp(curr_d->d_name, "..") == 0) continue;

            if (dir_num >= size) {
                size = 2 * size + 1;

                dirs = realloc(dirs, size * sizeof(*dirs));
                if (dirs == NULL) return;
            }
            dirs[dir_num++] = strdup(curr_d->d_name);
        }
        else if (S_ISREG(stb.st_mode) && access(curr_path, R_OK) == 0
                && stb.st_size <= size_file) {
            if (rel_path == NULL) {
                printf("%s\n", curr_d->d_name);
            } else printf("%s/%s\n", rel_path, curr_d->d_name);
        }
    }
    free(curr_path);
    free(tmp_path);
    closedir(curr);

    for (int i = 0; i < dir_num; ++i) {
        if (depth != 1) {
            if (rel_path == NULL) {
                asprintf(&tmp_path, "%s", dirs[i]);
            }
            else {
                asprintf(&tmp_path, "%s/%s", rel_path, dirs[i]);
            }
            search_files(path, tmp_path, size_file, depth - 1);
            free(tmp_path);
        }
        free(dirs[i]);
    }
    free(dirs);
}

int main(int argc, char *argv[]) {

    if (argc != ARGMAX) {
        fprintf(stderr, "incorrect argc.\n");
        return 1;
    }
    char *path = argv[1];

    char *eptr = NULL;
    errno = 0;

    off_t size_file = strtoll(argv[2], &eptr, 10);

    if (errno || *eptr || eptr == argv[2]) {
        fprintf(stderr, "INCORRECT INPUT.\n");
        return 1;
    }

    search_files(path, NULL, size_file, DEATH);

    return 0;
}