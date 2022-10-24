#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

const static char LIMIT[] = "18446744073709551616";

enum
{
    K = 1000
};

int
main(int argc, char *argv[])
{   
    long long sum = 0;
    for (int i = 1; i < argc; ++i) {

        char *eptr = NULL;
        errno = 0;

        long long tmp_l = strtoll(argv[i], &eptr, 10);
        if (errno || argv[i] == eptr || (int) tmp_l != tmp_l) continue;

        int tmp_i = (int) tmp_l;

        if (*eptr == 'k') { 
            if (__builtin_mul_overflow(tmp_i, K, &tmp_i)) continue;
            tmp_l = (long long) tmp_i;
            ++eptr;
        }
        if (*eptr == '+') sum += tmp_l;
        else if (*eptr == '-') sum -= tmp_l;
    }
    printf("%lld\n", sum);
    return 0;
}