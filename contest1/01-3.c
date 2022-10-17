#include <stdio.h>
#include <math.h>

enum
{
    SIZE_MAX = 2000
};

static char
is_goodnprime(int N)
{
    if (N <= 1 || N >= SIZE_MAX) return 0;

    for (int i = 2; i <= (int) sqrt(N); ++i) {
        if (N % i == 0) return 0;
    }
    return 1;
}

int
main(void)
{
    int data[SIZE_MAX] = {};
    int N;

    if (scanf("%d", &N) != 1 || !is_goodnprime(N)) {
        fprintf(stderr, "INPUT 1 POSITIVE PRIME NUMBER.\n");
        return 1;
    }

    for (int i = 0; i <= N - 1; ++i) {
        for (int j = 1; j <= N - 1; ++j) {
            int tmp;

            if (j == 1) tmp = i;
            else tmp = (N - (N / j) * data[N % j - 1] % N) % N;
            data[j - 1] = tmp;
            printf("%d ", tmp);
        }
        putchar('\n');
    }
    return 0;
}