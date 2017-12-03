#ifndef LINDA_H
#define LINDA_H

#include "tuple.h"

void linda_output(const tuple* value);
int linda_input(const tuple* pattern, tuple* output, unsigned timeout_ms);
int linda_read(const tuple* pattern, tuple* output, unsigned timeout_ms);

#endif /* end of include guard: LINDA_H */
