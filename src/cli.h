#ifndef CLI_H
#define CLI_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

void cli_start();
void run_command(struct parse_result* result);

#endif
