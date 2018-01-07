#ifndef TUPLE_H
#define TUPLE_H

#include "node.h"

#ifndef TUPLE_MAX_SIZE
#define TUPLE_MAX_SIZE 5
#endif

struct tuple {
    size_t size;
    struct node elems[TUPLE_MAX_SIZE];
};

extern struct tuple make_tuple();
extern int tuple_append(struct tuple *t, struct node node);
bool match_tuple(
    const struct tuple *pattern,
    const struct tuple *value
);

#endif /* end of include guard: TUPLE_H */
