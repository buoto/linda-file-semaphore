#include "node.h"

struct node make_string_node(const char* string, int size) {
    char* value = (char*) malloc(sizeof(char) * size);
    strncpy(value, string, size);
    return (struct node) {
        .type = STRING,
        .value = value,
    };
}

struct node make_int_node(long long integer) {
    return (struct node) {
        .type = INTEGER,
        .value = (void*) integer,
    };
}

void destroy_node(struct node* node) {
    if (node == NULL) {
        return;
    }

    if (node->type == STRING) {
        free(node->value);
    }
    node->value = NULL;
}

void print_node(struct node node) {
    switch (node.type) {
        case INTEGER:
            printf("%llu\n", (long long) node.value);
            return;
        case STRING:
            printf("%s\n", (char*) node.value);
            return;
    }
}

