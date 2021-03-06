#ifndef FILE_H
#define FILE_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>

#include "tuple.h"
#include "store.h"
#include "parser.h"

#define MAX_LINE_SIZE 1024

#define SEM_PATH_SUFFIX ".sem"

struct file {
    char *path;
    sem_t *sem;
};

struct file make_linda_file(char *path, size_t size);

int timed_lock(const struct file *f, const struct timespec *ts);
int lock(const struct file *f);
int unlock(const struct file *f);

int read_store_file(const struct file *f, struct store *s);
int write_store_file(const struct file *f, struct store *s);

void destroy_file(struct file *f);

#endif /* end of include guard: FILE_H */
