%{
    #include <stdio.h>
    #include "hanson/include/list.h"
    #include "hanson/include/mem.h"
    #include "hanson/include/assert.h"

    List_T ll = NULL;
    typedef struct Pair *Pair;
    struct Pair {char *key; int freq; };

    List_T List_find_string(List_T l, char *key);
    List_T List_add_pair(List_T l, char *key);
%}

WORD [a-zA-Z]
DIGIT [0-9]
WHITESPACE [ \t\n\r]

%%

{WORD}+	        {  
                  char *yycopy = strdup(yytext);
                  ll = List_add_pair(ll, yycopy); 
                }
{DIGIT}+	    	
{WHITESPACE}*   
.		

%%

void applyPrint(void **ptr, void *cl) {
    if (ptr) {
        Pair p = (Pair)*ptr;
        printf("%s\t%d\n", p->key, p->freq);
    }
    if (cl) {}
}

void applyFree(void **ptr, void *cl) {
    FREE(((Pair)*ptr)->key);
    FREE(*ptr);
    if (cl) {}
}

int compare(const void *x, const void *y) {
    return strcmp((*(Pair *)x)->key, (*(Pair *)y)->key);
}

List_T List_add_pair(List_T l, char *key) {
    List_T match = List_find_string(l, key);
    if (match) {
        FREE(key);
        ((Pair)match->first)->freq++;
        return l;
    }
    Pair p = NEW(p);
    p->key = key;
    p->freq = 1;
    
    return List_push(l, p);
}

List_T List_find_string(List_T l, char *key) {    
    for ( List_T next = l ; l ; l = next) {
        if ( strcmp(((Pair)l->first)->key, key) == 0 ) return l;
        next = l->rest;
    }
    return NULL;
}

int yywrap(){
    return 1;
}

int main() {

    yylex();

    Pair *array = (Pair *)List_toArray(ll, NULL);
    qsort((void **)array, List_length(ll), sizeof (*array), 
        (int (*)(const void *, const void*))compare);
    for (int i = 0; array[i]; i++) 
        printf("%s\t%d\n", array[i]->key, array[i]->freq);
    FREE(array);

    List_map(ll, applyFree, NULL);
    List_free(&ll);
    yylex_destroy();

    return 0;
}
