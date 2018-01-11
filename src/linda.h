#ifndef LINDA_H
#define LINDA_H

#define _GNU_SOURCE

#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>

#include "file.h"
#include "tuple.h"
#include "store.h"
#include "time_utils.h"

struct linda {
    sem_t *reader_mutex;
    sem_t *done_reading;
    sem_t *notify;
    sem_t *readers_count;

    struct file file;

    const char *reader_mutex_name;
    const char *done_reading_name;
    const char *notify_name;
    const char *readers_count_name;
};

struct linda *make_linda(const char *sem_base_path, struct file f);

int linda_output(const struct linda *l, const struct tuple* value);

int linda_input(
    const struct linda *l,
    const struct tuple* pattern,
    struct tuple* output,
    unsigned timeout_ms
);

int linda_read(
    const struct linda *l,
    const struct tuple* pattern,
    struct tuple* output,
    unsigned timeout_ms
);

void destroy_linda(struct linda *l);

#endif /* end of include guard: LINDA_H */
