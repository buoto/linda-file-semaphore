#ifndef LINDA_H
#define LINDA_H

#include "tuple.h"

void linda_output(struct tuple *value);
void linda_input(struct tuple *pattern, unsigned timeout_ms);
void linda_read(struct tuple *pattern, unsigned timeout_ms);

#endif /* end of include guard: LINDA_H */
