#include "cli.h"
#include "file.h"
#include "store.h"
#include "debug.h"

#define FILENAME "linda.file"

int main() {
    struct file f = make_linda_file(FILENAME, sizeof FILENAME);

    struct store s = make_store();
    read_store_file(&f, &s);

    timed_lock(&f, 10);
    write_store_file(&f, &s); // should create "linda.file" file with "asd\n" inside
    unlock(&f);

    struct cli cli = {
        .linda = NULL,
        .stream = stdin
    };
    cli_start(&cli);

    destroy_file(&f);
}
