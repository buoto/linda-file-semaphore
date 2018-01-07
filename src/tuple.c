#include "tuple.h"

struct tuple make_tuple() {
    return (struct tuple) {
        .size = 0
    };
}

int tuple_append(struct tuple *t, struct node node) {
    if (t == NULL) {
        return -1;
    }
    if (t->size == TUPLE_MAX_SIZE) {
        return -1;
    }

    t->elems[t->size++] = node;
    return 0;
}

bool match_tuple(
    const struct tuple *pattern,
    const struct tuple *value
) {
    return true;
}
