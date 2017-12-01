#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



enum node_type { INTEGER, STRING };

struct node {
    enum node_type type;
    void* value;
};

extern struct node make_string_node(const char* string, int size);
extern struct node make_int_node(long long integer);
extern void destroy_node(struct node* node);
extern void print_node(struct node node);


#endif /* end of include guard: NODE_H */
