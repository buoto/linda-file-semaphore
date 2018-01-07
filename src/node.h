#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum node_type { INTEGER, STRING };
enum node_matcher { EQUAL, LESSER, LESSER_OR_EQUAL, GREATER, GREATER_OR_EQUAL, ANY_INTEGER };

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

int match_pattern(const char *pattern, const char *value);

#endif /* end of include guard: NODE_H */
