#ifndef STORE_H
#define STORE_H

#include <stdbool.h>

#include "tuple.h"

struct store {
    struct store_node *first;
};

struct store make_store();

void store_append(struct store *s, struct tuple t);

struct tuple* find_in_store(const struct store *s, const struct tuple pattern, tuple_comparer f);
struct tuple pop_in_store(struct store *s, const struct tuple pattern, tuple_comparer f);

int fprintf_store(const struct store *s, FILE* stream);

void destroy_store(struct store *s);

#endif /* end of include guard: STORE_H */
