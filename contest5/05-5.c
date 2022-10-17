#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>

enum 
{
    BUF_MAX = 4000
};

int
copy_file(const char *srcpath, const char *dstpath)
{
    if (access(srcpath, R_OK)) return -1;

    int fin = open(srcpath, O_RDONLY);

    if (fin == -1) {
        return -1;
    }
    
    struct stat buf_in = {};
    stat(srcpath, &buf_in);

    if (!S_ISREG(buf_in.st_mode)) return -1;

    char *srcpath_tmp = strdup(srcpath);
    char *dstpath_tmp = strdup(dstpath);
    
    struct stat buf_out = {};
    stat(dstpath, &buf_out);
    
    if (S_ISDIR(buf_out.st_mode)) {
        char *file_name = basename(srcpath_tmp);

        strcat(dstpath_tmp, "/");
        strcat(dstpath_tmp, file_name);
    }

    stat(dstpath_tmp, &buf_out);

    if (buf_in.st_ino == buf_out.st_ino) {
        free(srcpath_tmp);
        free(dstpath_tmp);

        close(fin);
        return 0;
    }

    int fout = open(dstpath_tmp, O_CREAT | O_TRUNC | O_WRONLY, buf_in.st_mode);

    free(srcpath_tmp);
    free(dstpath_tmp);

    if (fout == -1) {
        return -1;
    }

    char buf[BUF_MAX] = {};
    ssize_t resd = 0, tmpd = 0;

    while ((resd = read(fin, buf, BUF_MAX)) > 0) {

        char *tmp_buf = buf;

        while (resd > 0) {

            if ((tmpd = write(fout, tmp_buf, resd)) == -1) {
                return -1;
            }
            resd -= tmpd;
            tmp_buf += tmpd;
        }
    }
    if (close(fin) == -1 || close(fout) == -1) {
        return -1;
    }
    return 0;
}

