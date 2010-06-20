#ifndef h_core_qutex_h
#define h_core_qutex_h

#include "h/core/bool.h"
#include "h/core/mutex.h"
#include "h/external/external.h"

struct h_core_qutex_t;
typedef struct h_core_qutex_t h_core_qutex_t;

h_core_qutex_t *h_core_qutex_create();

void h_core_qutex_destroy(h_core_qutex_t *qutex);

void h_core_qutex_lock_exclusive(h_core_qutex_t *qutex);

void h_core_qutex_lock_shared(h_core_qutex_t *qutex);

h_core_bool_t h_core_qutex_trylock_exclusive(h_core_qutex_t *qutex);

h_core_bool_t h_core_qutex_trylock_shared(h_core_qutex_t *qutex);

void h_core_qutex_unlock_exclusive(h_core_qutex_t *qutex);

void h_core_qutex_unlock_shared(h_core_qutex_t *qutex);

#endif
