#ifndef LINDA_H
#define LINDA_H

#define _GNU_SOURCE

#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>

#include "file.h"
#include "debug.h"
#include "tuple.h"
#include "store.h"
#include "time_utils.h"

#include "debug.h"

struct linda {
    sem_t *notify;
    sem_t *waiting;
    sem_t *waiting_guard;

    struct file file;

    char *notify_name;
    char *waiting_name;
    char *waiting_guard_name;
};

struct linda *make_linda(const char *sem_base_path, struct file f);

int wait_linda(const struct linda *l, const struct timespec *deadline);
void broadcast_linda(const struct linda *l);

int linda_output(const struct linda *l, const struct tuple *value);

int linda_input(
    const struct linda *l,
    const struct tuple *pattern,
    struct tuple *output,
    unsigned timeout_ms
);

int linda_read(
    const struct linda *l,
    const struct tuple *pattern,
    struct tuple *output,
    unsigned timeout_ms
);

void destroy_linda(struct linda *l);

#endif
