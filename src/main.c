#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tuple.h"

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
