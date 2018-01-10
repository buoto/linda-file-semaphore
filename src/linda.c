#include <stdio.h>
#include <string.h>
#include "linda.h"

#define READER_SEM_SUFFIX ".reader"
#define DONE_READING_SEM_SUFFIX ".done_reading"
#define NOTIFY_SEM_SUFFIX ".notify"


struct linda *make_linda(const char *sem_base_path, struct file f) {
    size_t path_len = strlen(sem_base_path);

    struct linda *l = (struct linda*) malloc(sizeof (struct linda));
    l->file = f;

    char *buf = (char*) malloc(path_len + sizeof(DONE_READING_SEM_SUFFIX));

    strcpy(buf, sem_base_path);
    strcat(buf, READER_SEM_SUFFIX);
    l->reader_mutex = sem_open(buf, O_CREAT, 0700, 1);

    strcpy(buf + path_len - 1, DONE_READING_SEM_SUFFIX);
    l->done_reading = sem_open(buf, O_CREAT, 0700, 1);

    strcpy(buf + path_len - 1, NOTIFY_SEM_SUFFIX);
    l->notify = sem_open(buf, O_CREAT, 0700, 1);

    free(buf);

    return l;
}

int linda_output(const struct linda *l, const struct tuple* value) {
    if(l == NULL || value == NULL) {
        return -1;
    }
    lock(&l->file);

    struct store s = make_store();

    read_store_file(&(l->file), &s);
    store_append(&s, *value);

    write_store_file(&(l->file), &s);

    sem_trywait(l->notify);
    while (errno == EAGAIN) {
        sem_post(l->notify);
        errno = 0;
        sem_trywait(l->notify);
    }

    unlock(&l->file);
}

int linda_input(
    const struct linda *l,
    const struct tuple* pattern,
    struct tuple* output,
    unsigned timeout_ms
) {
    printf("linda_input: not implemented/n");
    exit(1);
}

int linda_read(
    const struct linda *l,
    const struct tuple* pattern,
    struct tuple* output,
    unsigned timeout_ms
) {
    printf("linda_read: not implemented/n");
    exit(1);
}

void destroy_linda(struct linda *l) {
    if(l == NULL) {
        return;
    }

    sem_close(l->reader_mutex);
    sem_close(l->done_reading);
    sem_close(l->notify);

    free(l);
}
