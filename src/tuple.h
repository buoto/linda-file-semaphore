#ifndef TUPLE_H
#define TUPLE_H

#include "node.h"

#ifndef TUPLE_MAX_SIZE
#define TUPLE_MAX_SIZE 5
#endif

typedef struct {
    size_t size;
    node elems[TUPLE_MAX_SIZE];
} tuple;

extern tuple make_tuple();
extern int tuple_append(tuple *t, node node);

#endif /* end of include guard: TUPLE_H */
