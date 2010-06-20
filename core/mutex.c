#include "h/core/mutex.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_core_mutex_t {
  pthread_mutex_t mutex;
};

h_core_mutex_t *h_core_mutex_create()
{
  h_core_mutex_t *mutex;

  mutex = malloc(sizeof *mutex);
  if (mutex) {
    if (0 != pthread_mutex_init(&mutex->mutex, NULL)) {
      h_core_trace("pthread_mutex_init");
      free(mutex);
      mutex = NULL;
    }
  } else {
    h_core_trace("malloc() failed");
  }

  return mutex;
}

void h_core_mutex_destroy(h_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_destroy(&mutex->mutex)) {
    h_core_trace("pthread_mutex_destroy");
  }
  free(mutex);
}

void h_core_mutex_lock(h_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_lock(&mutex->mutex)) {
    h_core_trace("pthread_mutex_lock");
  }
}

h_core_bool_t h_core_mutex_trylock(h_core_mutex_t *mutex)
{
  assert(mutex);
  h_core_bool_t success;

  if (0 == pthread_mutex_lock(&mutex->mutex)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

void h_core_mutex_unlock(h_core_mutex_t *mutex)
{
  if (0 != pthread_mutex_unlock(&mutex->mutex)) {
    h_core_trace("pthread_mutex_unlock");
  }
}
