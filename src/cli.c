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
    }

    free(line);
}
