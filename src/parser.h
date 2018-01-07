#ifndef PARSER_H
#define PARSER_H

#include "tuple.h"

enum operation { INPUT, OUTPUT, READ, EXIT };

struct parse_result {
    enum operation operation;
    struct tuple tuple;
    unsigned timeout_ms;
    unsigned error;
};

struct parse_result parse(char* user_input);

#endif
