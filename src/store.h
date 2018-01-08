#ifndef STORE_H
#define STORE_H

#include <stdbool.h>

#include "tuple.h"

struct store {
    struct store_node *first;
};

struct store make_store();

void append_store(struct store *s, struct tuple t);

typedef bool (*tuple_comparer)(const struct tuple*, const struct tuple*); // TODO move to tuple
struct tuple* find_in_store(const struct store *s, const struct tuple pattern, tuple_comparer f);
struct tuple* pop_in_store(struct store *s, const struct tuple pattern, tuple_comparer f);

void destroy_store(struct store *s);

#endif /* end of include guard: STORE_H */
