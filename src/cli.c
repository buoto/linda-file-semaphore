#include "cli.h"

void cli_start(const struct cli *cli) {
    ssize_t nread;
    char* line = NULL;
    size_t len = 0;

    while((nread = getline(&line, &len, cli->stream)) != -1) {
        line[nread - 1] = 0;
        struct parse_result pr;
        int result = parse(&pr, line);

        if(result) {
            printf("%s\n", result);
            continue;
        }

        if(run_command(cli->linda, &pr) == 1) {
            break;
        }
    }

    free(line);
    printf("Goodbye :)\n");
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
}

void run_input_command(
    const struct linda *linda,
    const struct tuple *tuple,
    unsigned timeout_ms
) {
    struct tuple output;
    int result = linda_input(tuple, &output, timeout_ms);
}

void run_output_command(
    const struct linda *linda,
    const struct tuple *tuple
) {
    int result = linda_output(tuple);
}
