#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#define _GNU_SOURCE

#include <semaphore.h>
#include <time.h>

#define NS_IN_MS 1000000
#define NS_IN_S  1000000000
#define MS_IN_S  1000

struct timespec from_ms(unsigned ms);

struct timespec get_now();

int set_now(struct timespec *ts);

struct timespec add_ms(struct timespec *ts, long ms);

#endif /* end of include guard: TIME_UTILS_H */
