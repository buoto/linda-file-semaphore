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

typedef bool (*tuple_comparer)(const struct tuple*, const struct tuple*);

size_t tuple_length(const struct tuple *t);
size_t tuple_serialize(const struct tuple *t, char *out, size_t length);
#endif /* end of include guard: TUPLE_H */
