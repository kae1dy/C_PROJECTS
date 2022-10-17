#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

enum
{
    SIZE = 10000,
    CHANGE = 1,
    SUM = 2
};

int
sum_arr(const int *array, int L, int R) {

    int sum = 0;

    for (int i = L; i < R; ++i) {
        sum += array[i];
    }
    return sum;
}

void
change_arr(int *array, int L, int R, int S) {

    for (int i = L; i < R; ++i) {
        array[i] += S;
    }
}

int
main(void) {
    int N, M;

    if (scanf("%d%d", &N, &M) != 2 || 0 >= N || N > SIZE || 0 >= M) {
        fprintf(stderr, "INCORRECT INPUT.\n");
    }
    int array[SIZE] = {};
    int F, L, R, S;

    for (int i = 0; i < M; ++i) {

        scanf("%d", &F);

        switch (F) {
            case CHANGE:
                scanf("%d%d%d", &L, &R, &S);
                change_arr(array, L, R, S);
                break;
            case SUM:
                scanf("%d%d", &L, &R);
                printf("%d\n", sum_arr(array, L, R));
                break;
            default:
                break;
        }
    }
    return 0;
}