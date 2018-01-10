#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define NODE_I(V) { .type = INTEGER, .int_value = V, .matcher = EQUAL }
#define NODE_IM(V, M) { .type = INTEGER, .int_value = V, .matcher = M }
#define NODE_S(V) { .type = STRING, .str_value = V, .matcher = EQUAL }
#define NODE_SM(V, M) { .type = STRING, .str_value = V, .matcher = M }

enum node_type { INTEGER, STRING };
enum node_matcher { EQUAL, LESSER, LESSER_OR_EQUAL, GREATER, GREATER_OR_EQUAL, ANY };

struct node {
    enum node_type type;
    enum node_matcher matcher;
    char *str_value;
    unsigned int_value;
};

struct node make_string_node(const char *string, unsigned size);
struct node make_int_node(unsigned integer);
void destroy_node(struct node *node);
void print_node(struct node node);

bool match_node(
    const struct node *pattern,
    const struct node *value
);
bool match_pattern(const char *pattern, const char *value);
bool match_integer(
    enum node_matcher matcher,
    unsigned pattern,
    unsigned value
);
bool match_string(
    enum node_matcher matcher,
    const char *pattern,
    const char *value
);

size_t node_length(const struct node n);

#endif /* end of include guard: NODE_H */
