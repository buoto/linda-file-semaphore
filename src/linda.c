#include "linda.h"

#define READER_SEM_SUFFIX ".reader"
#define DONE_READING_SEM_SUFFIX ".done_reading"
#define NOTIFY_SEM_SUFFIX ".notify"
#define READERS_COUNT_SEM_SUFFIX ".notify"

char *make_name(const char *sem_base_path, const char *suffix) {
    char *buf = (char*) malloc(strlen(sem_base_path) + strlen(suffix));
    strcpy(buf, sem_base_path);
    strcat(buf, suffix);
    return buf;
}

struct linda *make_linda(const char *sem_base_path, struct file f) {
    struct linda *l = (struct linda*) malloc(sizeof (struct linda));
    l->file = f;

    l->reader_mutex_name = make_name(sem_base_path, READER_SEM_SUFFIX);
    l->reader_mutex = sem_open(l->reader_mutex_name, O_CREAT, 0700, 1);

    l->done_reading_name = make_name(sem_base_path, DONE_READING_SEM_SUFFIX);
    l->done_reading = sem_open(l->done_reading_name, O_CREAT, 0700, 1);

    l->notify_name = make_name(sem_base_path, NOTIFY_SEM_SUFFIX);
    l->notify = sem_open(l->notify_name, O_CREAT, 0700, 1);

    l->readers_count_name = make_name(sem_base_path, READERS_COUNT_SEM_SUFFIX);
    l->readers_count = sem_open(l->readers_count_name, O_CREAT, 0700, 1);

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
    return 0;
}

int linda_input(
    const struct linda *l,
    const struct tuple* pattern,
    struct tuple* output,
    unsigned timeout_ms
) {
    // sanity checks
    if(l == NULL || output == NULL || pattern == NULL) {
        return -1;
    }

    const struct file *linda_file = &(l->file);

    // get start value
    struct timespec start;
    if(set_now(&start) == -1) {
        return errno;
    }
    struct timespec deadline = add_ms(&start, timeout_ms); // deadline

    while(1) {
        if(sem_timedwait(l->reader_mutex, &deadline)) { // wait(reader_mutex)
            return errno; // error or timeout - abort
        }
        // reader_mutex PROTECTED
        sem_post(l->readers_count); // readers_count ++
        sem_post(l->reader_mutex); // signal(reader_mutex);
        // reader_mutex END

        struct store s = make_store();

        timed_lock(linda_file, &deadline); // rlock
        // file_lock PROTECTED
        int err = read_store_file(linda_file, &s); // operate
        unlock(linda_file); // runlock
        // file_lock END

        if(err) {
            /* Wrapping below trywait in mutex is unnecessary, because process can
             * either decrements what it has incremented or his increment was
             * consumed by someone else and trywait does nothing */
            sem_trywait(l->readers_count); // prompt decrement
            return err; // errors from read_store_file
        }

        struct tuple *target_tuple = find_in_store(&s, *pattern, match_tuple);
        destroy_store(&s);

        if(target_tuple != NULL) {
            // SUCCESS - tuple is found
            sem_trywait(l->readers_count); // prompt decrement
            *output = *target_tuple;
            return 0;
        }

        if(sem_timedwait(l->notify, &deadline)) { // wait(notify)
            sem_trywait(l->readers_count); // prompt decrement
            return errno;
        }

        if(sem_timedwait(l->reader_mutex, &deadline)) { // wait(reader_mutex)
            sem_trywait(l->readers_count); // prompt decrement
            return errno;
        }
        // reader_mutex PROTECTED
        sem_trywait(l->readers_count); // readers_count--

        if(sem_trywait(l->readers_count)) { // readers_count == 0 ?
            if(errno != EAGAIN) {
                // something really bad happened
                sem_post(l->reader_mutex);
                return errno;
            }
            // semaphore is closed => readers_count == 0
            // the last reader arrived, unblock all done_reading
            int done_reading_count;
            sem_getvalue(l->done_reading, &done_reading_count);
            while(done_reading_count < 0) { // count is negative or zero
                sem_post(l->done_reading);
                sem_getvalue(l->done_reading, &done_reading_count);
            }

            sem_post(l->reader_mutex);
            // reader_mutex END1
        } else {
            // early arrivers here
            sem_post(l->readers_count); // revert value after measurment

            sem_post(l->reader_mutex); // return the mutex
            // reader_mutex END2

            if(sem_timedwait(l->done_reading, &deadline)) { // wait for the others
                return errno;
            }
        }
    }
}

int linda_read(
    const struct linda *l,
    const struct tuple* pattern,
    struct tuple* output,
    unsigned timeout_ms
) {
    // sanity checks
    if(l == NULL || output == NULL || pattern == NULL) {
        return -1;
    }

    const struct file *linda_file = &(l->file);

    // get start value
    struct timespec start;
    if(set_now(&start) == -1) {
        return errno;
    }
    struct timespec deadline = add_ms(&start, timeout_ms); // deadline

    while(1) {
        if(sem_timedwait(l->reader_mutex, &deadline)) { // wait(reader_mutex)
            return errno; // error or timeout - abort
        }
        // reader_mutex PROTECTED
        sem_post(l->readers_count); // readers_count ++
        sem_post(l->reader_mutex); // signal(reader_mutex);
        // reader_mutex END

        struct store s = make_store();

        timed_lock(linda_file, &deadline); // rlock
        // file_lock PROTECTED
        int err = read_store_file(linda_file, &s); // operate
        unlock(linda_file); // runlock
        // file_lock END

        if(err) {
            /* Wrapping below trywait in mutex is unnecessary, because process can
             * either decrements what it has incremented or his increment was
             * consumed by someone else and trywait does nothing */
            sem_trywait(l->readers_count); // prompt decrement
            return err; // errors from read_store_file
        }

        struct tuple *target_tuple = find_in_store(&s, *pattern, match_tuple);
        destroy_store(&s);

        if(target_tuple != NULL) {
            // SUCCESS - tuple is found
            sem_trywait(l->readers_count); // prompt decrement
            *output = *target_tuple;
            return 0;
        }

        if(sem_timedwait(l->notify, &deadline)) { // wait(notify)
            sem_trywait(l->readers_count); // prompt decrement
            return errno;
        }

        if(sem_timedwait(l->reader_mutex, &deadline)) { // wait(reader_mutex)
            sem_trywait(l->readers_count); // prompt decrement
            return errno;
        }
        // reader_mutex PROTECTED
        sem_trywait(l->readers_count); // readers_count--

        if(sem_trywait(l->readers_count)) { // readers_count == 0 ?
            if(errno != EAGAIN) {
                // something really bad happened
                sem_post(l->reader_mutex);
                return errno;
            }
            // semaphore is closed => readers_count == 0
            // the last reader arrived, unblock all done_reading
            int done_reading_count;
            sem_getvalue(l->done_reading, &done_reading_count);
            while(done_reading_count < 0) { // count is negative or zero
                sem_post(l->done_reading);
                sem_getvalue(l->done_reading, &done_reading_count);
            }

            sem_post(l->reader_mutex);
            // reader_mutex END1
        } else {
            // early arrivers here
            sem_post(l->readers_count); // revert value after measurment

            sem_post(l->reader_mutex); // return the mutex
            // reader_mutex END2

            if(sem_timedwait(l->done_reading, &deadline)) { // wait for the others
                return errno;
            }
        }
    }
}

void destroy_linda(struct linda *l) {
    if(l == NULL) {
        return;
    }

    free(l->reader_mutex_name);
    free(l->done_reading_name);
    free(l->notify_name);
    free(l->readers_count_name);

    sem_close(l->reader_mutex);
    sem_close(l->done_reading);
    sem_close(l->notify);
    sem_close(l->readers_count);

    free(l);
}
