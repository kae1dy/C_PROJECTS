#include <stdio.h>
#include <limits.h>

const static char LIMIT[] = "18446744073709551616";

int
main(int argc, char *argv[])
{   
    char tmp, flag = 0;
    long long int sum = 0, sum_tmp;

    while (1) {
        tmp = getchar();
        
        if (tmp == ' ' || tmp == '\n' || tmp == EOF) {
            if (flag) printf("%lld\n", sum);
            if (tmp == EOF) return 0;
        
            flag = sum = 0;
            while ((tmp = getchar()) == ' ' ||tmp == '\n');

        }
        sum_tmp = sum;
        if (tmp == 'a') {
            flag = 1;
            if (__builtin_sub_overflow(sum, 1, &sum)
                    || __builtin_add_overflow(sum, sum_tmp, &sum) || __builtin_add_overflow(sum, sum_tmp, &sum)) {
                puts(LIMIT);
                flag = sum = 0;
                while ((tmp = getchar()) != ' ' && tmp != '\n' && tmp != EOF);
            }
        }
        else if (tmp == '1' || tmp == '0') {
            flag = 1;
            if (__builtin_add_overflow(sum, (long long) (tmp - '0'), &sum)
                    || __builtin_add_overflow(sum, sum_tmp, &sum) || __builtin_add_overflow(sum, sum_tmp, &sum)) {
                puts(LIMIT);
                flag = sum = 0;
                while ((tmp = getchar()) != ' ' && tmp != '\n' && tmp != EOF);
            }
        }
    }
    return 0;
}