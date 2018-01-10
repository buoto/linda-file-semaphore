#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#define _GNU_SOURCE

#include <semaphore.h>
#include <time.h>

struct timespec from_ms(unsigned ms);

time_t sem_timedwait_ms(sem_t *sem, time_t timeout_ms);


#endif /* end of include guard: TIME_UTILS_H */
