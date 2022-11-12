#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

typedef struct List *List;

struct List
{
    char str[FILENAME_MAX];
    List next;
    List prev;
    char notvalid;
};


void
cpyNfree(List head, char *buf) {
    if (head == NULL) return;

    char *buf_next = buf;

    while (head != NULL) {
        if (!head->notvalid) {
            buf_next = stpcpy(buf_next, "/");
            buf_next = stpcpy(buf_next, head->str);
        }
        List tmp = head;
        head = head->next;
        free(tmp);
    }

    if (buf != buf_next) buf_next[0] = 0;
    else buf[1] = 0;

}

void 
normalize_path(char *buf) {

    size_t size;
    if (buf == NULL || (size = strlen(buf)) == 0) return;

    List head = NULL, last = NULL, tmp = NULL;
    
    int counter = 0;

    for (int i = 0; i < size; ++i) {

        if (buf[i] != '/') {
            last->str[counter++] = buf[i];

        } else if (i != size - 1) {
            tmp = calloc(1, sizeof(*head));
            if (tmp == NULL) return;

            if (head == NULL) head = tmp;
            if (last != NULL) last->next = tmp;

            tmp->prev = last;
            last = tmp, counter = 0;
        }
    }
    counter = 0;
    char flag = 0;

    while (tmp != NULL) {

        if (strcmp(tmp->str, "..") == 0) {
            if (flag) counter = 0, flag = 0;
            tmp->notvalid = 1, ++counter;
        }
        else if (strcmp(tmp->str, ".") == 0) {
            tmp->notvalid = 1;
        }
        if (flag) {
            tmp->notvalid = 1;
            flag = 0, --counter;
        }
        if (counter > 0) {
            tmp->notvalid = 1;
            flag = 1, --counter;
        }
        tmp = tmp->prev;
    }
    cpyNfree(head, buf);
}

int
main(int argc, char *argv[])
{   
    char s[] = "/aa/bfdds/.";
    normalize_path(s);
    printf("%s", s);
}