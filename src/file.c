#include "file.h"

#define SEM_PATH_PREFIX ".sem"

struct file make_linda_file(char *path, size_t size) {
    char* value = (char*) malloc(sizeof(char) * size);
    strncpy(value, path, size);

    char* sem_path = (char*) malloc(
        sizeof(char) * (size + sizeof(SEM_PATH_PREFIX))
    );
    strncpy(sem_path, path, size);
    strncpy(sem_path + size - 1, SEM_PATH_PREFIX, sizeof(SEM_PATH_PREFIX));

    sem_t *sem = sem_open(sem_path, O_CREAT, 0700, 1);
    free(sem_path);

    return (struct file) {
        .path = value,
        .sem = sem,
    };
}

int timed_lock(struct file *f, const struct timespec *ts) {
    if(f == NULL || f->sem == NULL) {
        return -1;
    }
    return sem_timedwait(f->sem, ts);
}

int lock(struct file *f) {
    if(f == NULL || f->sem == NULL) {
        return -1;
    }
    return sem_wait(f->sem);
}

int unlock(struct file *f) {
    if(f == NULL || f->sem == NULL) {
        return -1;
    }
    return sem_post(f->sem);
}

int read_store_file(struct file *f, struct store *s) {
    if(f == NULL || f->path == NULL || s == NULL) {
        return -1; // NULL pointer
    }

    // open file
    FILE* fd = fopen(f->path, "r");
    if(fd == NULL) {
        return -2; // file read error
    }


    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &len, fd)) != -1) {
        struct tuple t = make_tuple();
        deserialize_tuple(&t, line);
        store_append(s, t);
    }

    fclose(fd);

    return 0;
}

int write_store_file(struct file *f, struct store *s) {
    if(f == NULL || f->path == NULL || s == NULL) {
        return -1; // NULL pointer
    }

    // open file
    FILE* stream = fopen(f->path, "w");
    if(stream == NULL) {
        return -2; // file read error
    }

    int result = fprintf_store(s, stream);

    fclose(stream);

    return result;
}

void destroy_file(struct file *f) {
    if(f == NULL) {
        return;
    }
    free(f->path);
    f->path = NULL;

    if (f->sem != NULL) {
        sem_close(f->sem);
        f->sem = NULL;
    }
}
