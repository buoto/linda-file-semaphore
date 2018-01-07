#include "cli.h"

int main() {
    struct cli cli = {
        .linda = NULL,
        .stream = stdin
    };
    cli_start(&cli);
}
