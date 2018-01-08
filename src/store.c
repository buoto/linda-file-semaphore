#include "store.h"

struct store_node {
    struct store_node *next;
    struct tuple element;
};

struct store make_store() {
    return (struct store) {NULL};
}

void append_store(struct store *s, struct tuple t) {
    if(s == NULL) {
        return;
    }
    struct store_node *node = (struct store_node*) malloc(sizeof(struct store_node));
    node->element = t;
    node->next = NULL;

    if(s->first == NULL) {
        s->first = node;
    } else {
        struct store_node *cur = s->first;
        while(cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node;
    }
}

struct tuple* find_in_store(
        const struct store *s,
        const struct tuple pattern,
        tuple_comparer f
) {
    if(s == NULL) {
        return NULL;
    }
    struct store_node *cur = s->first;

    while(cur != NULL) {
        if(f(&pattern, &cur->element)) {
            return &(cur->element);
        }

        cur = cur->next;
    }
    return NULL;
}

struct tuple* pop_in_store(
        struct store *s,
        const struct tuple pattern,
        tuple_comparer f
) {
    if(s == NULL) {
        return NULL;
    }
    struct store_node *cur = s->first, *prev = NULL;

    while(cur != NULL) {
        if(f(&pattern, &cur->element)) {
            if(prev == NULL) {
                s->first = cur->next;
            } else {
                prev->next = cur->next;
            }
            return &(cur->element); // mmr lik hr
        }

        prev = cur;
        cur = cur->next;
    }
    return NULL;
}

void destroy_store(struct store *s) {
    if(s == NULL) {
        return;
    }

    struct store_node *cur = s->first, *next;

    while(cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }

    s->first = NULL;
}
