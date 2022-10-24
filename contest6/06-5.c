#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static _Bool
isSame(struct stat stb1, struct stat stb2) {
    if (stb1.st_ino == stb2.st_ino && stb1.st_dev == stb2.st_dev) {
        return 1; 
    }
    return 0;
}

ssize_t getcwd2(int fd, char *buf, size_t size) {

    if (fd < 0) return -1;

    int fd_end = open("./", O_RDONLY);
    if (fd_end < 0) return -1;

    if (fchdir(fd) == -1) goto error;

    char res_path[PATH_MAX] = {};
    ssize_t size_res = 0;

    struct stat curr_st, file_st, root_st;

    if (lstat("/", &root_st) != 0 || lstat("./", &curr_st) != 0) goto error;
    
    while (!isSame(root_st, curr_st)) {

        DIR *updir = opendir("./../");
        if (updir == NULL) goto error;

        struct dirent *file_d;
        char *file_path = NULL;

        _Bool flag = 0;

        while ((file_d = readdir(updir)) != NULL) {
            
            free(file_path);
            asprintf(&file_path, "./../%s", file_d->d_name);

            if (lstat(file_path, &file_st) != 0) continue;
            
            if (S_ISDIR(file_st.st_mode)) {
                if (strcmp(file_d->d_name, "..") == 0) continue;
                if (strcmp(file_d->d_name, ".") == 0) continue;
                
                if ((flag = isSame(file_st, curr_st))) {
                    char *tmp = strdup(res_path);
                    size_res = snprintf(res_path, PATH_MAX, "/%s%s", file_d->d_name, tmp);
                    free(tmp);
                    break;
                }
            }
        }
        free(file_path);

        int updir_fd = dirfd(updir);
        if (fchdir(updir_fd) == -1) goto error;
        if (!flag || lstat("./", &curr_st) != 0) goto error;

        closedir(updir);   
    }

    fchdir(fd_end);

    if (size_res == 0) {
        res_path[0] = '/';
        ++size_res;
    }
    if (size >= 1 && buf != NULL) {
        strncpy(buf, res_path, size - 1);
        buf[size - 1] = 0;
    }
    return size_res;
    
    error:
    fchdir(fd_end);
    return -1;
}
