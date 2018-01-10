#include "time_utils.h"

struct timespec from_ms(unsigned ms) {
    return (struct timespec) {
        .tv_sec = ms/1000,
        .tv_nsec = (ms % 1000) * 1000000,
    };
}

time_t sem_timedwait_ms(sem_t *sem, time_t timeout_ms) {
    struct timespec start, end, ts = from_ms(timeout_ms);

    clock_gettime(CLOCK_MONOTONIC, &start);
    sem_timedwait(sem, &ts);
    clock_gettime(CLOCK_MONOTONIC, &end);

    time_t diff_sec = end.tv_sec - start.tv_sec;
    time_t diff_nsec = end.tv_nsec - start.tv_nsec;

    return (diff_sec * 1000) + (diff_nsec / 1000000); // diff in ms
}
