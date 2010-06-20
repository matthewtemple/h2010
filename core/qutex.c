#include "h/core/mutex.h"
#include "h/core/qutex.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_core_qutex_t {
  unsigned long shared_user_count;
  h_core_bool_t exclusive_user;
  h_core_mutex_t *mutex;
};

h_core_qutex_t *h_core_qutex_create()
{
  h_core_qutex_t *qutex;

  qutex = malloc(sizeof *qutex);
  if (qutex) {
    qutex->shared_user_count = 0;
    qutex->exclusive_user = h_core_bool_false;
    qutex->mutex = h_core_mutex_create();
    if (!qutex->mutex) {
      h_core_trace("h_core_mutex_create");
      free(qutex);
      qutex = NULL;
    }
  } else {
    h_core_trace("malloc() failed");
  }

  return qutex;
}

void h_core_qutex_destroy(h_core_qutex_t *qutex)
{
  assert(qutex);
  h_core_mutex_destroy(qutex->mutex);
  free(qutex);
}

void h_core_qutex_lock_exclusive(h_core_qutex_t *qutex)
{
  assert(qutex);
  while (!h_core_qutex_trylock_exclusive(qutex)) {}
}

void h_core_qutex_lock_shared(h_core_qutex_t *qutex)
{
  assert(qutex);
  while (!h_core_qutex_trylock_shared(qutex)) {}
}

h_core_bool_t h_core_qutex_trylock_exclusive(h_core_qutex_t *qutex)
{
  assert(qutex);
  h_core_bool_t locked;

  h_core_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user && (0 == qutex->shared_user_count)) {
      qutex->exclusive_user = h_core_bool_true;
      locked = h_core_bool_true;
    } else {
      locked = h_core_bool_false;
    }
  }
  h_core_mutex_unlock(qutex->mutex);

  return locked;
}

h_core_bool_t h_core_qutex_trylock_shared(h_core_qutex_t *qutex)
{
  assert(qutex);
  h_core_bool_t locked;

  h_core_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user) {
      qutex->shared_user_count++;
      locked = h_core_bool_true;
    } else {
      locked = h_core_bool_false;
    }
  }
  h_core_mutex_unlock(qutex->mutex);

  return locked;
}

void h_core_qutex_unlock_exclusive(h_core_qutex_t *qutex)
{
  h_core_mutex_lock(qutex->mutex);
  qutex->exclusive_user = h_core_bool_false;
  h_core_mutex_unlock(qutex->mutex);
}

void h_core_qutex_unlock_shared(h_core_qutex_t *qutex)
{
  h_core_mutex_lock(qutex->mutex);
  qutex->shared_user_count--;
  h_core_mutex_unlock(qutex->mutex);
}
