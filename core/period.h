#ifndef h_core_period_h
#define h_core_period_h

#include "h/core/types.h"
#include "h/external/external.h"

struct h_core_period_t;
typedef struct h_core_period_t h_core_period_t;

h_core_period_t *h_core_period_create(unsigned long seconds);

void h_core_period_destroy(h_core_period_t *period);

h_core_bool_t h_core_period_once(h_core_period_t *period);

#endif
