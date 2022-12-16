#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

enum { INPUT = 3, BITS = 8 };

int64_t minlld(int64_t x, int64_t y) {
    if (x == -1) return y;
    if (y == -1) return x;
    return (x > y) ? y : x;
}

int64_t 
calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count) {
    int64_t blocksSum = inode_direct_block_count, blocksK =  block_size / block_num_size;
    int64_t tmp = blocksK, numInd;

    numInd = (block_num_size >= BITS) ? -1 : (1LL << (block_num_size * BITS));

    if (numInd != -1 && __builtin_smulll_overflow(block_size, numInd, &numInd)) {
        numInd = -1;
    }
    if (__builtin_saddll_overflow(blocksSum, tmp, &blocksSum)) {
        blocksSum = -1;
    }
    else if (__builtin_smulll_overflow(blocksK, tmp, &tmp) || __builtin_saddll_overflow(blocksSum, tmp, &blocksSum)) {
        blocksSum = -1;
    }
    else if (__builtin_smulll_overflow(blocksK, tmp, &tmp) || __builtin_saddll_overflow(blocksSum, tmp, &blocksSum)) {
        blocksSum = -1;
    }
    else if (__builtin_smulll_overflow(blocksSum, block_size, &blocksSum)) {
        blocksSum = -1;
    }
    return minlld(blocksSum, numInd);
}

int
main (int argc, char *argv[])
{   
    int64_t block_size, block_num_size, inode;
    if (scanf("%lld %lld %lld", &block_size, &block_num_size, &inode) != INPUT) {
        fprintf(stderr, "incorrect input.\n");
        exit(EXIT_FAILURE);
    }
    printf("%lld\n", calc_max_size(block_size, block_num_size, inode));
    
    int64_t lastVal = -2, temp;
    for (int64_t i = 1; i < BITS; ++i) {
        temp = calc_max_size(block_size, i, inode);
        if (temp == -1) {
            printf("%lld %lld\n", i, temp);
            break;
        }
        if (lastVal == -1 || lastVal >= temp) {
            printf("%lld %lld\n", i - 1, lastVal);
            break;
        }
        lastVal = temp;
        if (i == BITS - 1) {
            printf("%lld %lld\n", i, lastVal);
        }
    }
    exit(EXIT_SUCCESS);
}