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

struct timespec add_ms(const struct timespec *ts, long ms);

time_t sem_timedwait_ms(sem_t *sem, time_t timeout_ms);

#endif /* end of include guard: TIME_UTILS_H */
