#ifndef PARSER_H
#define PARSER_H

#include "tuple.h"

enum operation { INPUT, OUTPUT, READ };

struct parse_result {
    enum operation operation;
    struct tuple tuple;
    unsigned timeout_ms;
    char* error;
};

struct parse_result parse(char* user_input);

#endif
