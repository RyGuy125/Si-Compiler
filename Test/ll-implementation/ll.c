#include <assert.h>
#include <stdlib.h>
#include "ll.h"

#define T List_T

T List_new() {
    T p;
    p = malloc(sizeof *p);
    assert(p);
    p->first=NULL;
    p->rest=NULL;
    return p;
}

T List_append(T list, T tail) {
    T *p = &list;
    while (*p) p = &(*p)->rest;
    *p = tail;
    return list;
}

T List_pop(T list, void **x) {
    if (list) {
        T head = list->rest;
        if (x) *x = list->first;
        free(list);
        return head;
    } else return list;
}

T List_push(T list, void *x) {
    T p;

    p = malloc(sizeof *p);
    assert(p);
    p->first = x;
    p->rest = list;
    return p;
}

void List_free(T *list) {
    T next;

    assert(list);
    for ( ; *list; *list = next) {
        next = (*list)->rest;
        free(*list);
    }
}

void List_map(T list, void apply(void **x, void *cl), void *cl) {
    assert(apply);
    for ( ; list; list = list->rest) apply(&list->first, cl);
}