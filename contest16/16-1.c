#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

enum { INPUT = 5, ARG_NUM = 4 };

void setVal(int *arr, int size, int val) {
    for (int i = 0; i < size; ++i) {
        arr[i] = val;
    }
}

int
main (int argc, char *argv[])
{   
    if (argc != ARG_NUM) {
        fprintf(stderr, "incorrect args.\n");
        exit(EXIT_FAILURE);
    }
    int64_t cache_size, block_size, count = 0;

    errno = 0;
    char *eptr = NULL;
    cache_size = strtol(argv[2], &eptr, 10);
    if (errno || *eptr || argv[2] == eptr) exit(EXIT_FAILURE);

    errno = 0;
    eptr = NULL;
    block_size = strtol(argv[3], &eptr, 10);
    if (errno || *eptr || argv[3] == eptr) exit(EXIT_FAILURE);

    int cacheSize = cache_size / block_size;
    int *cacheArr = calloc(cacheSize, sizeof(*cacheArr));
    setVal(cacheArr, cacheSize, -1);
    
    char mode, buf;
    int phys_address, size, value;
    while (scanf("%c%c %x %d %d\n", &mode, &buf, &phys_address, &size, &value) == INPUT) {

        if (mode == 'R') {
            int phys_block_num = phys_address / block_size;
            int cache_block_num = phys_block_num % cacheSize;

            if (cacheArr[cache_block_num] != -1 && cacheArr[cache_block_num] != phys_block_num ) {
                ++count;
            }
            cacheArr[cache_block_num] = phys_block_num;
        }
    }
    printf("%lld\n", count);
    free(cacheArr);
    exit(EXIT_SUCCESS);
}