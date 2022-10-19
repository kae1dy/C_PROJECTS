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
                sum = 0, flag = 1;
            }
            flag = 2;
            continue;

        } if (!flag) continue;

        if (tmp != 'a' && tmp != '1' && tmp != '0') continue;

        if (__builtin_mul_overflow(sum, 3, &sum)) {
            puts(LIMIT);
            sum = 0, flag = 0;
            continue;
        }
        if (tmp == 'a') {
            flag = 1;
            if (__builtin_sub_overflow(sum, 1, &sum)) {
                puts(LIMIT);
                sum = 0, flag = 0;
                continue;
            }
        } else if (tmp == '0' || tmp == '1') {
            flag = 1;
            if (__builtin_add_overflow(sum, (long long) (tmp - '0'), &sum)) {
                puts(LIMIT);
                sum = 0, flag = 0;
            }
        } 
    }
    return 0;
}