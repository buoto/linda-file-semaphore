#include "tuple.h"

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

bool match_tuple(
    const struct tuple *pattern,
    const struct tuple *value
) {
    if(pattern->size != value->size) {
        return false;
    }
    for(unsigned i = 0; i < pattern->size; i++) {
        if(!match_node(&pattern->elems[i], &value->elems[i])) {
            return false;
        }
    }
    return true;
}

#define BRACKET_LEN 2
#define COMA_LEN 1
#define QUOTE_LEN 2

size_t tuple_length(const struct tuple *t) {
    if(t == NULL) {
        return 0;
    }

    size_t length = BRACKET_LEN;
    for(unsigned i = 0; i < t->size; i++) {
        length += node_length(t->elems[i]);

        if(t->elems[i].type == STRING) {
            length += QUOTE_LEN;
        }
    }
    if(t->size > 0) {
        length += COMA_LEN * (t->size - 1); // add commas
    }
    return length;
}

size_t tuple_serialize(const struct tuple *t, char *out, size_t length) {
    int iter = 0;
    out[iter++] = '(';
    for(unsigned i = 0; i < t->size; i++) {
        struct node n = t->elems[i];
        int n_len = node_length(n);

        int result = 0;
        switch(n.type) {
            case STRING:
                out[iter++] = '"';
                strncpy(out + iter, n.str_value, n_len);
                iter += n_len;
                out[iter++] = '"';
                break;
            case INTEGER:
                result = sprintf(out + iter, "%d", n.int_value);
                iter += n_len;
                break;
        }
        if(result < 0) {
            return result; // handle error
        }

        out[iter++] = ',';
    }
    if(t->size > 0) {
        iter--; // overwrite last comma
    }
    out[iter++] = ')';
    out[iter++] = 0;

    return iter; // return written bytes
}
