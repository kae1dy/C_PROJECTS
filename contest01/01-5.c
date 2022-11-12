#include <stdio.h>

enum
{
    SIZE = 10,
    TRUE = 1,
    FALSE = 0
};

static void print_array(const int *array, size_t n) {
    for (int i = 0; i < n; ++i) {
        printf("%d", array[i]);
    }
    putchar('\n');
}

static void
perm(int index, int n, int *ans, char *is_used) {
    if (index >= n) {
        print_array(ans, n);
        return;
    }
    for (int i = 1; i <= n; ++i) {
        if (!is_used[i]) {
            is_used[i] = TRUE;
            ans[index] = i;
            perm(index + 1, n, ans, is_used);
            is_used[i] = FALSE;
        }
    }
}

int main(void) {
    char is_used[SIZE] = {};
    int ans[SIZE] = {};

    int n;

    if (scanf("%d", &n) != 1 || n <= 0 || n >= 10) {
        fprintf(stderr, "INCORRECT INPUT.\n");
        return 1;
    }
    perm(0, n, ans, is_used);

    return 0;
}
