#include "cli.h"

void cli_start(const struct cli *cli) {
    ssize_t nread;
    char* line = NULL;
    size_t len = 0;

    printf("Welcome to multiprocess linda system.\nType 'exit' to quit.\n> ");

    while((nread = getline(&line, &len, cli->stream)) != -1) {
        line[nread - 1] = 0;
        struct parse_result pr;
        int error = parse(&pr, line);

        if(error) {
            print_error(error, line);
        } else if(run_command(cli->linda, &pr) == 1) {
            break;
        }

        printf("> ");
    }

    free(line);
    printf("Goodbye :)\n");
}

void print_error(int iter, const char* str) {
    printf("Error: %s\n      ", str);
    for(int i = 0; i < iter; i++)
        printf(" ");
    printf("^\n");
}

int run_command(
    const struct linda *linda,
    const struct parse_result *result
) {
    switch(result->operation) {
        case READ:
            run_read_command(linda, &result->tuple, result->timeout_ms);
            break;
        case INPUT:
            run_input_command(linda, &result->tuple, result->timeout_ms);
            break;
        case OUTPUT:
            run_output_command(linda, &result->tuple);
            break;
        case EXIT:
            return 1;
    }
    return 0;
}

void run_read_command(
    const struct linda *linda,
    const struct tuple *tuple,
    unsigned timeout_ms
) {
    struct tuple output;
    int result = linda_read(tuple, &output, timeout_ms);
    if(result != 0) {
        printf("Operation timed out.\n");
    } else {
        char tuple_str[1024];
        tuple_serialize(&output, tuple_str, 1024);
        printf("Result: %s\n", tuple_str);
    }
}

void run_input_command(
    const struct linda *linda,
    const struct tuple *tuple,
    unsigned timeout_ms
) {
    struct tuple output;
    int result = linda_input(tuple, &output, timeout_ms);
    if(result != 0) {
        printf("Operation timed out.\n");
    } else {
        char tuple_str[1024];
        tuple_serialize(&output, tuple_str, 1024);
        printf("Result: %s\n", tuple_str);
    }
}

void run_output_command(
    const struct linda *linda,
    const struct tuple *tuple
) {
    int result = linda_output(tuple);
    if(result != 0) {
        printf("Operation failed.\n");
    } else {
        printf("Operation successful.\n");
    }
}
