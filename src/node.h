#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum node_type { INTEGER, STRING };
enum node_matcher { NONE, LESSER, GREATER, LESSER_OR_EQUAL, GREATER_OR_EQUAL, EQUAL, ANY };

typedef struct {
    enum node_type type;
    enum node_matcher matcher;
    void* value;
} node;

extern node make_string_node(const char* string, int size);
extern node make_int_node(long long integer);
extern void destroy_node(node* node);
extern void print_node(node node);


#endif /* end of include guard: NODE_H */
