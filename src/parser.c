#include "parser.h"

struct parse_result parse(char* user_input) {
    struct parse_result result = (struct parse_result) {
        .operation = EXIT
    };
    return result;
}
