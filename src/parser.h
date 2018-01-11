#ifndef PARSER_H
#define PARSER_H

#include "tuple.h"

enum operation { INPUT, OUTPUT, READ, EXIT, CLEANUP };

struct parse_result {
    enum operation operation;
    struct tuple tuple;
    unsigned timeout_ms;
};

int parse(struct parse_result *res, char *user_input);
int parse_node(struct node *n, int *iter, const char *str);

int deserialize_tuple(struct tuple *t, const char *str);

#endif
