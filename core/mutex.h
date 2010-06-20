#ifndef mutex_h
#define mutex_h

#include "h/core/bool.h"
#include "h/external/external.h"

struct h_core_mutex_t;
typedef struct h_core_mutex_t h_core_mutex_t;

h_core_mutex_t *h_core_mutex_create();

void h_core_mutex_destroy(h_core_mutex_t *mutex);

void h_core_mutex_lock(h_core_mutex_t *mutex);

h_core_bool_t h_core_mutex_trylock(h_core_mutex_t *mutex);

void h_core_mutex_unlock(h_core_mutex_t *mutex);

#endif
