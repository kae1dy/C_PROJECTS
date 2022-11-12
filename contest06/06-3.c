#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

enum 
{
    LAG = 1900
};

int main()
{   
    int YEAR;
    scanf ("%d", &YEAR);

    struct tm val = {};
    val.tm_year = YEAR - LAG;
    val.tm_mday = 1;
    val.tm_isdst = -1;

    errno = 0;
    mktime(&val);
    //err
    if (errno) {
        fprintf(stderr, "incorrect time.\n");
    }
    int month = val.tm_mon;
    int count = 0;

    while (val.tm_year == YEAR - LAG) {

        if (val.tm_wday == 4) {
            if (val.tm_mday % 3 && count % 2) {
                printf("%d %d\n", val.tm_mon + 1, val.tm_mday);
            }
            ++count;
        }
        ++val.tm_mday;
        val.tm_isdst = -1;
    
        errno = 0;
        mktime(&val);
        if (errno) {
            fprintf(stderr, "incorrect time.\n");
        }
        if (month != val.tm_mon) {
            month = val.tm_mon;
            count = 0;
        }
    }

    return 0;
}