#include <stdio.h>

enum
{
    NULL_3BIT = 59,
    BIG_LET = 'A' - '9' + '0' - 'z' + 'a' - 3,
    LOW_LET = 'a' - '9' + '0' - 2,
    DIG = '0' - 1,
};

int
main(void)
{
    int tmp;
    char alph[] = "@0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#";
    while ((tmp = getchar()) != EOF) {

        if ((tmp >= 'A') && (tmp <= 'Z')) {
            tmp -= BIG_LET;
            tmp &= NULL_3BIT;
            tmp ^= 1 << 3;
        } else if ((tmp >= 'a') && (tmp <= 'z')) {
            tmp -= LOW_LET ;
            tmp &= NULL_3BIT;
            tmp ^= 1 << 3;
        } else if ((tmp >= '0') && (tmp <= '9')){
            tmp -= DIG;
            tmp &= NULL_3BIT;
            tmp ^= 1 << 3;
        } else continue;
        putchar(alph[tmp]);
    }
    return 0;
}