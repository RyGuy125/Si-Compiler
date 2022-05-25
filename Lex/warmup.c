#include "A_grammar.h"
#include <stdio.h>
#include <stdlib.h>

#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; })

////////////////////////////////////////////////////////

typedef struct table *Table_;
struct table {string id; int value; Table_ tail;};
Table_ Table(string id, int value, struct table *tail) {
    Table_ t = malloc(sizeof(*t));
    t->id = id;
    t->value = value;
    t->tail = tail;
    return t;
}

Table_ update(Table_ t, string id, int value) {
    return Table(id, value, t);
}

int lookup(Table_ t, string key) {
    Table_ next = t;
    while(next) {
        if (key == t->id) return t->value;
        else next = next->tail;
    }
    fprintf(stderr, "Couldn't locate %s\n", key);
    exit(1);
}

/////////////////////////////////////////////////
Table_ interpStm(A_stm s, Table_ t);

struct IntAndTable {int i; Table_ t;};
struct IntAndTable interpExp(A_exp e, Table_ t) {
    if (e->kind == A_idExp) {
        struct IntAndTable it = { .i = lookup(t, e->u.id), .t = t};
        return it;
    }
    else if (e->kind == A_numExp) {
        struct IntAndTable it = {.i = e->u.num, .t = t};
        return it;
    }
    else if (e->kind == A_opExp) {
        struct IntAndTable it1 = interpExp(e->u.op.left, t);
        struct IntAndTable it2 = interpExp(e->u.op.right, it1.t);
        if (e->u.op.oper == A_plus) {
            it2.i += it1.i;
            return it2;
        }
        else if (e->u.op.oper == A_minus) {
            it2.i = it1.i - it2.i;
            return it2;
        }
        else if (e->u.op.oper == A_times) {
            it2.i *= it1.i;
            return it2;
        }
        else {
            it2.i = it1.i / it2.i;
            return it2;
        }
    }
    else {
        return interpExp(e->u.eseq.exp, interpStm(e->u.eseq.stm, t));
    }
}

struct IntAndTable interpExpList(A_expList es, Table_ t) {
    if (es->kind == A_lastExpList) {
            struct IntAndTable it = interpExp(es->u.last, t);
            printf("%d\n", it.i);
            return it;
    }
    struct IntAndTable it = interpExp(es->u.pair.head, t);
    printf("%d\n", it.i);
    return interpExpList(es->u.pair.tail, it.t);
}

Table_ interpStm(A_stm s, Table_ t) {
    if (s->kind == A_compoundStm) {
        return interpStm(s->u.compound.stm2, 
                              interpStm(s->u.compound.stm1, t));
    }
    else if (s->kind == A_assignStm) { 
        struct IntAndTable it = interpExp(s->u.assign.exp, t);
        return update(it.t, s->u.assign.id, it.i);
    }
    else {
        struct IntAndTable it = interpExpList(s->u.print.exps, t);
        return it.t;
    }
}

void interp(A_stm s) {
    Table_ t = interpStm(s, NULL);
    while (t) {
        printf("%s\t%d\n", t->id, t->value);
        t = t->tail;
    }
    printf("\n");
}


///////////////////////////////////////////////////

int maxargs(A_stm);

int maxargsExp(A_exp e) {
    if (e->kind == A_opExp) {
        return max(maxargsExp(e->u.op.left), maxargsExp(e->u.op.right));
    }
    else if (e->kind == A_eseqExp) {
        return max(maxargs(e->u.eseq.stm), maxargsExp(e->u.eseq.exp));
    }
    return 0;
}

int maxargsExpList(A_expList es) {
    if (es->kind == A_lastExpList) {
        return max(1, maxargsExp(es->u.last));
    }
    return max(maxargsExp(es->u.pair.head), 
                           1 + maxargsExpList(es->u.pair.tail));
}

// maxargs
// returns largest number of expressions within a print statement
int maxargs(A_stm s) {
    if (s->kind == A_compoundStm) {
        return max(maxargs(s->u.compound.stm1), maxargs(s->u.compound.stm2));
    }
    else if (s->kind == A_assignStm) {
        return maxargsExp(s->u.assign.exp);
    }
    else if (s->kind == A_printStm) {
        return maxargsExpList(s->u.print.exps);
    }
    return 0;
}


////////////////////////////////////////////////////

int main() {
    A_stm prog = 
    A_CompoundStm(A_AssignStm("a",
                    A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
     A_CompoundStm(A_AssignStm("b",
        A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"),
                    A_LastExpList(A_OpExp(A_IdExp("a"), A_minus, 
                                          A_NumExp(1))))),
                A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
      A_PrintStm(A_LastExpList(A_IdExp("b")))));

    A_stm prog1 = A_AssignStm("b",A_NumExp(6));

    A_stm prog2 = A_PrintStm(
                    A_PairExpList(
                        A_EseqExp(A_PrintStm(A_LastExpList(A_NumExp(10))),
                                    A_OpExp(A_NumExp(1), 
                                            A_minus, 
                                            A_IdExp("c"))),
                        A_LastExpList(A_IdExp("c"))
                    )
                );

    A_stm prog3 = A_PrintStm(
                    A_LastExpList(
                        A_EseqExp(A_AssignStm("d", A_NumExp(13)),
                            A_OpExp(A_EseqExp(
                                        A_PrintStm(A_LastExpList(A_IdExp("d"))), 
                                            A_IdExp("d")), 
                                        A_times, 
                                        A_NumExp(4)))));

    printf("%d\t%d\t%d\n", maxargs(prog), maxargs(prog1), maxargs(prog2));
    printf("%d\n", maxargs(prog3));
    interp(prog);
    interp(prog1);
    interp(prog3);
    interp(prog2);

    return 0;
    
}