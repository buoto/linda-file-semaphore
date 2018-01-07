#include "node.h"

struct node make_string_node(const char* string, unsigned size) {
    char* value = (char*) malloc(sizeof(char) * size);
    strncpy(value, string, size);
    return (struct node) {
        .type = STRING,
        .str_value = value,
    };
}

struct node make_int_node(unsigned integer) {
    return (struct node) {
        .type = INTEGER,
        .int_value = integer,
    };
}

void destroy_node(struct node* node) {
    if (node == NULL) {
        return;
    }

    if (node->type == STRING) {
        free(node->str_value);
    }
    node->str_value = NULL;
}

void print_node(struct node node) {
    switch (node.type) {
        case INTEGER:
            printf("%llu\n", node.int_value);
            return;
        case STRING:
            printf("%s\n", node.str_value);
            return;
    }
}

#define WILDCARD '*'
int match_pattern(const char *pattern, const char *value) {
    unsigned pattern_index = 0;
    unsigned value_index = 0;

    while(1) {
        char pattern_char = pattern[pattern_index];
        char value_char = value[value_index];

        switch(pattern_char) {
            case 0:
                return value_char == 0;
            case WILDCARD:
                if(match_pattern(
                    &pattern[pattern_index+1],
                    &value[value_index]
                )) {
                    return 1;
                } else {
                    value_index++;
                    break;
                }
            default:
                if(pattern_char != value_char) {
                    return 0;
                }
                pattern_index++;
                value_index++;
        }

        if(value_char == 0) {
            return 0;
        }
    }
}
