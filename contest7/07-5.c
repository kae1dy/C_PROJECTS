#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

enum 
{
    LAG = 1900,
    MOD = 2551440,
    N = 4
};

int main(void)
{   
    //26 05 2021 11:14:00 UTC (FIRST full moon)
    struct tm first_event = {};
    first_event.tm_year = 2021 - LAG;
    first_event.tm_mon = 4;
    first_event.tm_mday = 26;
    first_event.tm_hour = 11;
    first_event.tm_min = 14;
    first_event.tm_isdst = -1;

    time_t event = timegm(&first_event);

    int YEAR;
    scanf("%d", &YEAR);

    // 14 SEPT
    struct tm val = {};
    val.tm_year = YEAR - LAG;
    val.tm_mday = 257;
    val.tm_isdst = -1;

    errno = 0;
    time_t time_tmp = timegm(&val);

    if (time_tmp == -1 && errno) {
        fprintf(stderr, "incorrect time.\n");
        return 1;
    }
    time_tmp = (time_tmp - event) % MOD;

    if (time_tmp >= 0) val.tm_sec += (MOD - time_tmp);
    else val.tm_sec += time_tmp;

    val.tm_mday += 1;

    for (int i = N; ; ++val.tm_mday) {
        val.tm_isdst = -1;
        errno = 0;
        if (timegm(&val) == -1 && errno) return 1;
        if (val.tm_wday == 1) --i;
        if (i == 0) break;
    }
    printf("%04d-%02d-%02d\n", val.tm_year + LAG, val.tm_mon + 1, val.tm_mday);
    return 0;
}