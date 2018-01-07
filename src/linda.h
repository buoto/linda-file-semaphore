#ifndef LINDA_H
#define LINDA_H

#include "tuple.h"

int linda_output(const struct tuple* value);
int linda_input(const struct tuple* pattern, struct tuple* output, unsigned timeout_ms);
int linda_read(const struct tuple* pattern, struct tuple* output, unsigned timeout_ms);

#endif /* end of include guard: LINDA_H */
