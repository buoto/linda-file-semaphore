#include "linda.h"

#define WAITING_GUARD_SEM_SUFFIX ".waiting_guard"
#define WAITING_SEM_SUFFIX ".waiting"
#define NOTIFY_SEM_SUFFIX ".notify"

char *make_name(const char *sem_base_path, const char *suffix) {
    char *buf = (char*) malloc(strlen(sem_base_path) + strlen(suffix));
    strcpy(buf, sem_base_path);
    strcat(buf, suffix);
    return buf;
}

struct linda *make_linda(const char *sem_base_path, struct file f) {
    struct linda *l = (struct linda*) malloc(sizeof (struct linda));
    l->file = f;

    l->waiting_guard_name = make_name(sem_base_path, WAITING_GUARD_SEM_SUFFIX);
    l->waiting_guard = sem_open(l->waiting_guard_name, O_CREAT, 0700, 1);

    l->waiting_name = make_name(sem_base_path, WAITING_SEM_SUFFIX);
    l->waiting = sem_open(l->waiting_name, O_CREAT, 0700, 0);

    l->notify_name = make_name(sem_base_path, NOTIFY_SEM_SUFFIX);
    l->notify = sem_open(l->notify_name, O_CREAT, 0700, 0);

    return l;
}

int wait_linda(const struct linda *l, const struct timespec *deadline) {
    sem_wait(l->waiting_guard); {
        sem_post(l->waiting);
    } sem_post(l->waiting_guard);

    return sem_timedwait(l->notify, deadline);
}

void broadcast_linda(const struct linda *l) {
    sem_wait(l->waiting_guard); {
        int value;
        sem_getvalue(l->waiting, &value);
        for(; value > 0; value--) {
            sem_wait(l->waiting);
            sem_post(l->notify);
        }
    } sem_post(l->waiting_guard);
}

int linda_output(const struct linda *l, const struct tuple *value) {
    if(l == NULL || value == NULL) {
        return -1;
    }
    lock(&l->file);

    struct store s = make_store();

    read_store_file(&(l->file), &s);
    store_append(&s, *value);

    write_store_file(&(l->file), &s);

    broadcast_linda(l);

    unlock(&l->file);
    return 0;
}

struct timespec ms_from_now(unsigned timeout_ms) {
    struct timespec start;
    if(set_now(&start) == -1) {
        LOG("cannot set time\n");
        exit(1);
    }
    return add_ms(&start, timeout_ms);
}

int linda_input(
    const struct linda *l,
    const struct tuple *pattern,
    struct tuple *output,
    unsigned timeout_ms
) {
    // sanity checks
    if(l == NULL || output == NULL || pattern == NULL) {
        return -1;
    }

    const struct file *linda_file = &(l->file);
    struct timespec deadline = ms_from_now(timeout_ms);

    while(1) {
        lock(linda_file); {
            struct store s = make_store();
            int err = read_store_file(linda_file, &s);

            if(err) {
                unlock(linda_file);
                return err;
            }

            struct tuple target_tuple = pop_in_store(&s, *pattern, match_tuple);
            if(target_tuple.size != -1) {
                unlock(linda_file);
                write_store_file(&l->file, &s);
                destroy_store(&s);
                *output = target_tuple;
                return 0;
            }

            destroy_store(&s);
        } unlock(linda_file);

        wait_linda(l, &deadline);
    }
}

int linda_read(
    const struct linda *l,
    const struct tuple *pattern,
    struct tuple *output,
    unsigned timeout_ms
) {
    // sanity checks
    if(l == NULL || output == NULL || pattern == NULL) {
        return -1;
    }

    const struct file *linda_file = &(l->file);
    struct timespec deadline = ms_from_now(timeout_ms);

    while(1) {
        struct store s = make_store();

        lock(linda_file); {
            int err = read_store_file(linda_file, &s);
            if(err) {
                unlock(linda_file);
                return err;
            }
        } unlock(linda_file);


        struct tuple *target_tuple = find_in_store(&s, *pattern, match_tuple);
        destroy_store(&s);

        if(target_tuple != NULL) {
            *output = *target_tuple;
            return 0;
        }

        wait_linda(l, &deadline);
    }
}

void destroy_linda(struct linda *l) {
    if(l == NULL) {
        return;
    }

    free(l->waiting_guard_name);
    free(l->waiting_name);
    free(l->notify_name);

    sem_close(l->waiting_guard);
    sem_close(l->waiting);
    sem_close(l->notify);

    free(l);
}
