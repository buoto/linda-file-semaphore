#include "cli.h"

void cli_start() {
    ssize_t nread;
    char* line = NULL;
    size_t len = 0;

    while((nread = getline(&line, &len, stdin)) != -1) {
        line[nread - 1] = 0;
        struct parse_result result = parse(line);

        if(result.error) {
            printf("%s\n", result.error);
            continue;
        }

        run_command(&result);
    }

    free(line);
}

void run_command(struct parse_result* result) {
    switch(result->operation) {
        case INPUT:
            run_input_command(&result->tuple, result->timeout_ms);
    }
}

void run_input_command(struct tuple* tuple, unsigned timeout_ms) {
    struct tuple result;
    int error = linda_input(tuple, &result, timeout_ms);
}
