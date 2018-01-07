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

        if(result.operation == EXIT) {
            return;
        }

        run_command(&result);
    }

    free(line);
    printf("Goodbye :)");
}

int run_command(struct parse_result* result) {
    switch(result->operation) {
        case READ:
            run_read_command(&result->tuple, result->timeout_ms);
            break;
        case INPUT:
            run_input_command(&result->tuple, result->timeout_ms);
            break;
        case OUTPUT:
            run_output_command(&result->tuple);
            break;
    }
}

void run_read_command(struct tuple* tuple, unsigned timeout_ms) {
    struct tuple output;
    int result = linda_read(tuple, &output, timeout_ms);
}

void run_input_command(struct tuple* tuple, unsigned timeout_ms) {
    struct tuple output;
    int result = linda_input(tuple, &output, timeout_ms);
}

void run_output_command(struct tuple* tuple) {
    int result = linda_output(tuple);
}
