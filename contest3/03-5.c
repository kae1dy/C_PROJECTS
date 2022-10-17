#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *
dup_elem(struct Elem *head)
{
    struct Elem *prev = NULL, *tmp_head = head;

    for (; tmp_head != NULL; tmp_head = tmp_head->next) {

        char *eptr = NULL;
        errno = 0;
        int64_t tmp = strtol(tmp_head->str, &eptr, 10);

        if (errno || *eptr || eptr == tmp_head->str || (int) tmp != tmp) {
            prev = tmp_head;
            continue;
        }
        int res = (int) tmp;
        if (__builtin_add_overflow(res, (int) 1, &res)) {
            prev = tmp_head;
            continue;
        }
        char *str = NULL;
        asprintf(&str, "%d", res);

        struct Elem *tmp_ptr = calloc(1, sizeof(*tmp_ptr));
        tmp_ptr->str = str;
        tmp_ptr->next = tmp_head;

        if (prev != NULL) {
            prev->next = tmp_ptr;
        } else {
            head = tmp_ptr;
        }
        prev = tmp_head;
    }
    return head;
}
