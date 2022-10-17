#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

typedef struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
} Node;

Node *
nodeFromBin(int fbin, int index, Node *dist)
{
    if (index == 0 || dist == NULL) return NULL;

    lseek(fbin, (off_t) (index * sizeof(*dist)), SEEK_SET);

    if (read(fbin, dist, sizeof(*dist)) != sizeof(*dist)) {
        return NULL;
    }
    return dist;
}

void
inorderBT(int fbin, Node *head)
{
    if (head == NULL) return;

    Node *left_node = calloc(1, sizeof(*left_node));
    Node *right_node = calloc(1, sizeof(*right_node));

    left_node = nodeFromBin(fbin, head->left_idx, left_node);
    right_node = nodeFromBin(fbin, head->right_idx, right_node);

    inorderBT(fbin, right_node);
    printf("%d ", head->key);
    inorderBT(fbin, left_node);

    free(left_node);
    free(right_node);
}

int
main(int argc, char *argv[])
{
    int fbin = open(argv[argc - 1], O_RDONLY);

    // open file
    if (fbin == -1) {
        fprintf(stderr, "open file err\n");
        exit(1);
    }
    Node *head = calloc(1, sizeof(*head));

    if (read(fbin, head, sizeof(*head)) != sizeof(*head)) {
        exit(0);
    }
    inorderBT(fbin, head);
    putchar('\n');

    free(head);

    // close file
    if (close(fbin) == -1) {
        fprintf(stderr, "close file err\n");
        exit(1);
    }
    return 0;
}