/* tree.c */
#include "util.h"
#include <stdio.h>

typedef struct tree *T_tree;
struct tree {T_tree left; string key; void *binding; T_tree right; };

T_tree Tree(T_tree l, string k, void *b, T_tree r) {
    T_tree t = checked_malloc(sizeof(*t));
    t->left = l;
    t->key = k;
    t->binding = b;
    t->right = r;
    return t;
}

T_tree insert(string key, void *binding, T_tree t) {
    if (t == NULL) return TREE(NULL,key,binding,NULL);
    else if (strcmp(key,t->key) < 0)
        return Tree(insert(key,binding,t->left),t->key,t->binding,t->right);
    else if (strcmp(key,t->key) > 0)
        return Tree(t->left,t->key,t->binding,insert(key,binding,t->right));
    else return Tree(t->left,key,binding,t->right);
}

bool member(string key, T_tree t) {
    if (t == NULL) return 0;
    if (strcmp(key, t->key) == 0) return 1;
    return strcmp(key, t->key) < 0 
        ? member(key, t->left) : member(key, t->right);
}

void *lookup(string key, T_tree t) {
    if (t == NULL) return NULL;
    if (strcmp(key, t->key) == 0) return t->binding;
    return strcmp(key, t->key) < 0 
        ? lookup(key, t->left) : lookup(key, t->right);
}