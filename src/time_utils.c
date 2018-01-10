#include "time_utils.h"

struct timespec from_ms(unsigned ms) {
    return (struct timespec) {
        .tv_sec = ms/1000,
        .tv_nsec = (ms % 1000) * 1000000,
    };
}

struct timespec get_now() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts;
}

struct timespec add_ms(const struct timespec *ts, long ms) {
    long nsec = ts != NULL ? ts->tv_nsec : 0;
    nsec += ms * NS_IN_MS; // add millis

    time_t sec = ts != NULL ? ts->tv_sec : 0;
    sec += nsec / NS_IN_S; // handle overflow
    nsec = nsec % NS_IN_S;

    return (struct timespec) {
        .tv_sec = sec,
        .tv_nsec = nsec,
    };
}

time_t sem_timedwait_ms(sem_t *sem, time_t timeout_ms) {
    struct timespec start, end, ts = from_ms(timeout_ms);

    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("%d %d\n",ts.tv_sec, ts.tv_nsec);
    int x = sem_timedwait(sem, &ts);
    printf("sem %d\n", sem);
    perror();
    clock_gettime(CLOCK_MONOTONIC, &end);

    time_t diff_sec = end.tv_sec - start.tv_sec;
    time_t diff_nsec = end.tv_nsec - start.tv_nsec;

    return (diff_sec * 1000) + (diff_nsec / 1000000); // diff in ms
}
