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

int set_now(struct timespec *ts) {
    return clock_gettime(CLOCK_REALTIME, ts);
}

struct timespec add_ms(struct timespec *ts, long ms) {
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
