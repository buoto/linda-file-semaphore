#ifndef CLI_H
#define CLI_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "linda.h"

void cli_start();
void run_command(struct parse_result* result);
void run_input_command(struct tuple* tuple, unsigned timeout_ms);

#endif
