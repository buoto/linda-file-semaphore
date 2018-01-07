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
            printf("%u\n", node.int_value);
            return;
        case STRING:
            printf("%s\n", node.str_value);
            return;
    }
}

bool match_node(
    const struct node *pattern,
    const struct node *value
) {
    if(pattern->type != value->type) {
        return false;
    }

    if(pattern->type == INTEGER) {
        return match_integer(
            pattern->matcher,
            pattern->int_value,
            value->int_value
        );
    } else {
        return match_string(
            pattern->matcher,
            pattern->str_value,
            value->str_value
        );
    }
}

bool match_integer(
    enum node_matcher matcher,
    unsigned pattern,
    unsigned value
) {
    switch(matcher) {
        case ANY:
            return true;
        case EQUAL:
            return value == pattern;
        case LESSER:
            return value < pattern;
        case LESSER_OR_EQUAL:
            return value <= pattern;
        case GREATER:
            return value > pattern;
        case GREATER_OR_EQUAL:
            return value >= pattern;
        default:
            return false;
    }
}

bool match_string(
    enum node_matcher matcher,
    const char *pattern,
    const char *value
) {
    switch(matcher) {
        case ANY:
            return true;
        case EQUAL:
            return match_pattern(pattern, value);
        case LESSER:
            return strcmp(value, pattern) < 0;
        case LESSER_OR_EQUAL:
            return strcmp(value, pattern) <= 0;
        case GREATER:
            return strcmp(value, pattern) > 0;
        case GREATER_OR_EQUAL:
            return strcmp(value, pattern) >= 0;
        default:
            return false;
    }
}

#define WILDCARD '*'
bool match_pattern(const char *pattern, const char *value) {
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
                    return true;
                } else {
                    value_index++;
                    break;
                }
            default:
                if(pattern_char != value_char) {
                    return false;
                }
                pattern_index++;
                value_index++;
        }

        if(value_char == 0) {
            return false;
        }
    }
}

