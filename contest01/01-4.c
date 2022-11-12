#include <stdio.h>
#include <stdlib.h>

int
comp(const void *x, const void *y)
{
    int *X = (int *) x;
    int *Y = (int *) y;

    if (*X % 2 && *Y % 2) {
        return (*Y > *X) - (*Y < *X);
    } else if (!(*X % 2 || *Y % 2)) {
        return (*X > *Y) - (*X < *Y);
    } else if (*X % 2) {
        return 1;
    }
    return -1;
}

void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, sizeof (*data), comp);
}