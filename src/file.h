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

#include "tuple.h"
#include "store.h"
#include "parser.h"

#define MAX_LINE_SIZE 1024

struct file {
    char *path;
    sem_t *sem;
};

struct file make_linda_file(char *path, size_t size);

int timed_lock(struct file *f, unsigned timeout_ms);
int unlock(struct file *f);

int read_store_file(struct file *f, struct store *s);
int write_store_file(struct file *f, struct store *s);

void destroy_file(struct file *f);

#endif /* end of include guard: FILE_H */
