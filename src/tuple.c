#include "tuple.h"

tuple make_tuple() {
    return (tuple) {
        .size = 0
    };
}

int tuple_append(tuple *t, node node) {
    if (t == NULL) {
        return -1;
    }
    if (t->size == TUPLE_MAX_SIZE) {
        return -1;
    }

    t->elems[t->size++] = node;
    return 0;
}

