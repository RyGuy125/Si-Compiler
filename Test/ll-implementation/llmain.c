#include "ll.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Pair *Pair;    
struct Pair {
   int value;
   char *word;
};

void print_pair(void **x, void *cl) {
    Pair p = (Pair)x;
    printf("%d\t%s", p->value, p->word);
}

void add(void **p, void *a) {
    (*(Pair *)p)->value+=(*(int *)a);
}

int main (int argc, char *argv[]) {
    List_T ll = List_new();

    Pair pair;
    pair = malloc(sizeof *pair);
    pair->value = 0;
    pair->word = "balls";

    ll = List_push(ll, pair);

    printf("%d\t%s\n",((Pair)ll->first)->value,((Pair)ll->first)->word);

    List_map(ll, print_pair, NULL);
    //List_map(ll, add, (void *)2);

    printf("%d\t%s\n",((Pair)ll->first)->value,((Pair)ll->first)->word);

    free(pair);
    List_free(&ll);

    return 0;
}