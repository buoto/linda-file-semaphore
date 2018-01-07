#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum node_type { INTEGER, STRING };
enum node_matcher { NONE, LESSER, GREATER, LESSER_OR_EQUAL, GREATER_OR_EQUAL, EQUAL, ANY };

struct node {
    enum node_type type;
    enum node_matcher matcher;
    void* value;
};

struct node make_string_node(const char *string, int size);
struct node make_int_node(long long integer);
void destroy_node(struct node *node);
void print_node(struct node node);

int match_pattern(const char *pattern, const char *value);

#endif /* end of include guard: NODE_H */
