#include <stdio.h>

const static char LIMIT[] = "18446744073709551616";

int
main(int argc, char *argv[])
{   
    char tmp, flag = 2;
    long long int sum = 0;

    while ((tmp = getchar()) != EOF) {
        
        if (tmp == ' ' || tmp == '\n') {
            if (flag == 1) {
                printf("%lld\n", sum);
                sum = 0;
            }
            flag = 2;
            continue;

        }
        if (!flag) continue;

        if (tmp != 'a' && tmp != '1' && tmp != '0') {
            if (flag == 1) printf("%lld\n", sum);
            sum = 0, flag = 2;
            continue;
        }
        long long int sum_tmp = sum;
        flag = 1;
        if (tmp == 'a') {
            if (__builtin_sub_overflow(sum, 1, &sum)) {
                puts(LIMIT);
                sum = 0, flag = 0;
            }
        } else if (tmp == '0' || tmp == '1') {
            if (__builtin_add_overflow(sum, (long long) (tmp - '0'), &sum)) {
                puts(LIMIT);
                sum = 0, flag = 0;
            }
        }
        if (__builtin_add_overflow(sum, sum_tmp, &sum) || __builtin_add_overflow(sum, sum_tmp, &sum)) {
            puts(LIMIT);
            sum = 0, flag = 0;
        }
    }
    return 0;
}