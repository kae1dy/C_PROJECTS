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
    SIZE = 65536,
    ARG = 3,
    OFFSET = 9,
    MASK_OFFSET = (1U << OFFSET) - 1
};

int
main(int argc, char *argv[])
{   
    if (argc != ARG) {
        fprintf(stderr, "Incorrect number of args.\n");
        exit(1);
    }
    
    char *eptr = NULL;
    errno = 0;
    unsigned long table_add = strtoul(argv[argc - 1], &eptr, 16);
    if (errno || *eptr || argv[argc - 1] == eptr ||
            (unsigned short) table_add != table_add) {
        fprintf(stderr, "Incorrect arg. \n");
        exit(1);
    }

    int fd = open(argv[argc - 2], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "file open err\n");
        exit(1);
    }

    unsigned short *buf = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (buf == MAP_FAILED) {
        fprintf(stderr, "Mapping Failed.\n");
        exit(1);
    }
    if (close(fd) < 0) {
        fprintf(stderr, "file close err\n");
        exit(1);
    } 
    unsigned short *table = buf + table_add / sizeof(*table);
    unsigned short virtual_add, phys_add;

    while (scanf("%hx", &virtual_add) == 1) {
        phys_add = table[virtual_add >> OFFSET] / sizeof(*table);
        
        printf("%u\n", buf[phys_add + (virtual_add & MASK_OFFSET) / sizeof(*buf)]);
    }
    if (munmap(buf, SIZE) != 0) {
        fprintf(stderr, "UnMapping Failed. \n");
        exit(1);
    }
    exit(0);
}