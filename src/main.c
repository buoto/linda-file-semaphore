#include "cli.h"
#include "file.h"
#include "store.h"
#include "linda.h"
#include "debug.h"

#define FILENAME "linda.file"
#define SEM_BASE_PATH "linda"

int main() {
    struct file f = make_linda_file(FILENAME, sizeof FILENAME);
    struct linda *l = make_linda(SEM_BASE_PATH, f);

    struct cli cli = {
        .linda = l,
        .stream = stdin
    };

    cli_start(&cli);

    destroy_linda(l);
    destroy_file(&f);
}
