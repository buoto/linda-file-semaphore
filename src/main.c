#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TUPLE_MAX_SIZE
#define TUPLE_MAX_SIZE 5
#endif

enum node_type { INTEGER, STRING };

struct node {
    enum node_type type;
    void* value;
};

struct tuple {
    size_t size;
    struct node elems[TUPLE_MAX_SIZE];
};

struct tuple make_tuple() {
    return (struct tuple) {
        .size = 0
    };
}

int tuple_append(struct tuple *t, struct node node) {
    if (t == NULL) {
        return -1;
    }
    if (t->size == TUPLE_MAX_SIZE) {
        return -1;
    }

    t->elems[t->size++] = node;
    return 0;
}

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

int main() {
    int integer = 123;
    struct node int_node = make_int_node(integer);
    print_node(int_node);

    char str[] = "Hello Buoto";
    struct node string_node = make_string_node(str, sizeof str);
    print_node(string_node);

    struct tuple t = make_tuple();

    tuple_append(&t, int_node);
    tuple_append(&t, string_node);

    for (unsigned i = 0; i < t.size; i++) {
        print_node(t.elems[i]);
    }

    return 0;
}
