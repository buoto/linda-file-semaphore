#ifndef CLI_H
#define CLI_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "linda.h"

void cli_start();
int run_command(struct parse_result* result);
void run_read_command(struct tuple* tuple, unsigned timeout_ms);
void run_input_command(struct tuple* tuple, unsigned timeout_ms);
void run_output_command(struct tuple* tuple);

#endif
