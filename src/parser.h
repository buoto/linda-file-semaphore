#ifndef PARSER_H
#define PARSER_H

#include "tuple.h"

enum operation { INPUT, OUTPUT, READ };

struct parse_result {
    enum operation operation;
    struct tuple tuple;
    int timeout; // if -1 infinite timeout
    char* error;
};

struct parse_result parse(char* user_input);

#endif PARSER_H
