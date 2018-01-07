#ifndef CLI_H
#define CLI_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "linda.h"

struct cli {
    struct linda *linda;
    FILE *stream;
};

void cli_start(const struct cli *cli);
int run_command(
    const struct linda *linda,
    const struct parse_result *result
);
void run_read_command(
    const struct linda *linda,
    const struct tuple *tuple,
    unsigned timeout_ms
);
void run_input_command(
    const struct linda *linda,
    const struct tuple *tuple,
    unsigned timeout_ms
);
void run_output_command(
    const struct linda *linda,
    const struct tuple *tuple
);

#endif
