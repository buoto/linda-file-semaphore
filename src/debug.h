#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <time.h>

#define LOG(...) do {char buf[100];  time_t now = time (0);  strftime (buf, 100, "%Y-%m-%d %H:%M:%S", localtime (&now)); printf("[%s %s:%d: %s()] ", buf, __FILE__, __LINE__, __func__); printf(__VA_ARGS__);} while(0)

#ifdef DEBUG
#define LOGD(...) LOG(__VA_ARGS__)
#else
#define LOGD(...)
#endif

#endif /* end of include guard: DEBUG_H */
