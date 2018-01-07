#include "cli.h"
#include "file.h"
#include "debug.h"

int main() {
    struct file f;
    int r = open_linda_file(&f, "linda.flock");
    LOGD("%d\n", r);

    struct cli cli = {
        .linda = NULL,
        .stream = stdin
    };
    cli_start(&cli);
}
