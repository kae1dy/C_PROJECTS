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
#include <sys/mman.h>

enum 
{ 
    ARG_NUM = 4,
    MODE = 0600
};

static int 
filetosize(int fd, off_t size) 
{
    if (lseek(fd, (off_t) size - 1, SEEK_SET) == -1) {
        return -1;
    }
    if (write(fd, "", sizeof("")) != 1) {
        return -1;
    }
    return 0;
}

static int 
min(int x, int y) 
{
    return (x > y) ? y : x;
}

static void 
spiralmat(int *matrix, int rows, int cols) 
{
    int N = min(rows, cols) / 2 + min(rows, cols) % 2;
    int counter = 1;

    for (int ring = 0; ring < N; ++ring) {

        for (int i = ring; i < cols - ring; ++i) { // left -> right
            matrix[cols * ring + i] = counter++;
        }
        for (int i = ring + 2; i < rows - ring; ++i) { // top -> down
            matrix[cols * i - ring - 1] = counter++;
        }
        if (rows - 2 * ring - 1 <= 0) break;

        for (int i = cols - ring - 1; i >= ring ; --i) { // right -> left
            matrix[cols * (rows - ring - 1) + i] = counter++;
        }
        if (cols - 2 * ring - 1 <= 0) break;

        for (int i = rows - ring - 2; i > ring; --i) { // down -> top

            matrix[cols * i + ring] = counter++;
        }
    }
}

int
main(int argc, char *argv[])
{
    if (argc != ARG_NUM) {
        fprintf(stderr, "Incorrect number of args.\n");
        exit(1);
    }
    
    char *eptr = NULL;
    errno = 0;
    long int cols = strtol(argv[argc - 1], &eptr, 10);
    if (errno || *eptr || argv[argc - 1] == eptr || (int) cols != cols || cols <= 0) {
        fprintf(stderr, "Incorrect arg. \n");
        exit(1);
    }
    long int rows = strtol(argv[argc - 2], &eptr, 10);
    if (errno || *eptr || argv[argc - 2] == eptr || (int) rows != rows || rows <= 0) {
        fprintf(stderr, "Incorrect arg. \n");
        exit(1);
    }

    int fd = open(argv[argc - 3], O_RDWR | O_TRUNC | O_CREAT, MODE);
    if (fd == -1) {
        fprintf(stderr, "File open error. \n");
        exit(1);
    }

    int *buf;
    ssize_t size = cols * rows * sizeof(*buf);
    
    if (filetosize(fd, (off_t) size) != 0) {
        fprintf(stderr, "lseek/write error. \n");
        exit(1);
    }
    
    buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (buf == MAP_FAILED) {
        fprintf(stderr, "Mapping Failed.\n");
        exit(1);
    }
    spiralmat(buf, rows, cols);

    if (munmap(buf, size) != 0) {
        fprintf(stderr, "UnMapping Failed. \n");
        exit(1);
    }
    exit(0);
}