#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <errno.h>

#define MAX_LINE_SIZE 1024

enum lock_status = {NONE, RLOCK, LOCK};

struct file {
    FILE *fd;
    enum lock_status locked;
};

int open_linda_file(struct file *f, char *path);

int timed_lock(struct file *f, unsigned timeout_ms);
int unlock(struct file *f);

int timed_read_lock(struct file *f, unsigned timeout_ms);
int read_unlock(struct file *f);

int read_tuple(struct file *f, struct tuple pattern, struct tuple *out);
int remove_tuple(struct file *f, struct tuple pattern);
int write_tuple(struct file *f, struct tuple tuple);

void destroy_file(struct file *f);

#endif /* end of include guard: FILE_H */
