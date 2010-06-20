#include "h/container/cache.h"
#include "h/core/constants.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/xcache/item.h"
#include "h/xcache/system.h"

#define WAIT_ON_OBJECT_SLEEP_MICROSECONDS H_CORE_STANDARD_SLEEP_MICROSECONDS

struct h_xcache_system_t {
  h_container_cache_t *container_cache;
  pthread_mutex_t mutex;
};

static void *find_copy_expire(h_xcache_system_t *system,
    h_core_uuid_t *object_uuid);

void *find_copy_expire(h_xcache_system_t *system,
    h_core_uuid_t *object_uuid)
{
  assert(system);
  assert(object_uuid);
  h_xcache_item_t *item_decoy;
  h_xcache_item_t *item;
  void *object_copy;

  object_copy = NULL;

  item_decoy = h_xcache_item_create_decoy(object_uuid);
  if (item_decoy) {

    pthread_mutex_lock(&system->mutex);
    {
      item = h_container_cache_find(system->container_cache, item_decoy);
      if (item) {
        object_copy = h_xcache_item_get_object_copy(item);
        if (!object_copy) {
          h_core_trace("h_xcache_item_get_object_copy");
        }
        h_xcache_item_expire(item);
      }
    }
    pthread_mutex_unlock(&system->mutex);

    h_xcache_item_destroy_decoy(item_decoy);
  } else {
    h_core_trace("h_xcache_item_create_decoy");
  }

  return object_copy;
}

h_core_bool_t h_xcache_system_add(h_xcache_system_t *system,
    h_core_uuid_t *object_uuid, void *object,
    h_core_copy_f copy, h_core_destroy_f destroy,
    unsigned long lifetime_seconds)
{
  assert(object_uuid);
  assert(object);
  assert(copy);
  h_core_bool_t success;
  h_xcache_item_t *item;

  success = h_core_bool_false;

  item = h_xcache_item_create(object_uuid, object, copy, destroy,
      lifetime_seconds);
  if (item) {
    success = h_container_cache_add(system->container_cache, item);
    if (!success) {
      h_xcache_item_destroy(item);
      h_core_trace("h_container_cache_add");
    }
  } else {
    h_core_trace("h_xcache_item_create");
  }

  return success;
}

void h_xcache_system_clear(h_xcache_system_t *system)
{
  h_container_cache_clear(system->container_cache);
}

h_xcache_system_t *h_xcache_system_create()
{
  h_xcache_system_t *system;
  h_core_bool_t so_far_so_good;
  h_core_bool_t mutex_needs_destroy;

  mutex_needs_destroy = h_core_bool_false;

  system = malloc(sizeof *system);
  if (system) {
    system->container_cache = h_container_cache_create(h_xcache_item_compare,
        h_xcache_item_copy, h_xcache_item_destroy, h_xcache_item_is_expired);
    if (system->container_cache) {
      so_far_so_good = h_core_bool_true;
    } else {
      so_far_so_good = h_core_bool_false;
      h_core_trace("new");
    }
  } else {
    so_far_so_good = h_core_bool_false;
    h_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (0 == pthread_mutex_init(&system->mutex, NULL)) {
      mutex_needs_destroy = h_core_bool_true;
    } else {
      so_far_so_good = h_core_bool_false;
      h_core_trace("pthread_mutex_init");
    }
  }

  if (system && !so_far_so_good) {
    if (system->container_cache) {
      h_container_cache_destroy(system->container_cache);
    }
    if (mutex_needs_destroy) {
      if (0 != pthread_mutex_destroy(&system->mutex)) {
        h_core_trace("pthread_mutex_destroy");
      }
    }
    free(system);
    system = NULL;
  }

  return system;
}

void h_xcache_system_destroy(h_xcache_system_t *system)
{
  assert(system);
  h_container_cache_destroy(system->container_cache);
  if (0 != pthread_mutex_destroy(&system->mutex)) {
    h_core_trace("pthread_mutex_destroy");
  }
  free(system);
}

void *h_xcache_system_find_wait_copy_expire
(h_xcache_system_t *system, h_core_uuid_t *object_uuid,
    unsigned long max_wait_seconds)
{
  assert(system);
  assert(object_uuid);
  void *object_copy;
  time_t start_wait_time;
  time_t current_time;
  unsigned long wait_time_seconds;

  start_wait_time = time(NULL);

  do {
    object_copy = find_copy_expire(system, object_uuid);
    if (!object_copy) {
      usleep(WAIT_ON_OBJECT_SLEEP_MICROSECONDS);
      current_time = time(NULL);
      wait_time_seconds = current_time - start_wait_time;
    }
  } while (!object_copy && (wait_time_seconds < max_wait_seconds));

  return object_copy;
}

unsigned long h_xcache_system_get_size(h_xcache_system_t *system)
{
  return h_container_cache_get_size(system->container_cache);
}

void h_xcache_system_remove_objects(h_xcache_system_t *system)
{
  pthread_mutex_lock(&system->mutex);
  h_container_cache_remove_objects(system->container_cache);
  pthread_mutex_unlock(&system->mutex);
}
