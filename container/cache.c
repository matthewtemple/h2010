#include "h/container/cache.h"
#include "h/container/set.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_container_cache_t {
  h_container_set_t *objects;
  h_core_qutex_t *objects_qutex;
  h_core_condition_f remove_condition;
};

h_core_bool_t h_container_cache_add(h_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  h_core_bool_t success;

  h_core_qutex_lock_exclusive(cache->objects_qutex);
  success = h_container_set_add(cache->objects, object);
  h_core_qutex_unlock_exclusive(cache->objects_qutex);

  if (!success) {
    h_core_trace("h_container_set_add");
  }

  return success;
}

void h_container_cache_clear(h_container_cache_t *cache)
{
  h_core_qutex_lock_exclusive(cache->objects_qutex);
  h_container_set_clear(cache->objects);
  h_core_qutex_unlock_exclusive(cache->objects_qutex);
}

h_container_cache_t *h_container_cache_create
(h_core_compare_f compare, h_core_copy_f copy,
    h_core_destroy_f destroy,
    h_core_condition_f remove_condition)
{
  h_container_cache_t *cache;
  h_core_bool_t so_far_so_good;

  cache = malloc(sizeof *cache);
  if (cache) {
    cache->remove_condition = remove_condition;
    cache->objects_qutex = NULL;
    cache->objects = h_container_set_create(compare, copy,
        destroy);
    if (cache->objects) {
      so_far_so_good = h_core_bool_true;
    } else {
      so_far_so_good = h_core_bool_false;
      h_core_trace("h_container_set_create");
    }
  } else {
    so_far_so_good = h_core_bool_false;
    h_core_trace("malloc() failed");
  }

  if (so_far_so_good) {
    cache->objects_qutex = h_core_qutex_create();
    if (!cache->objects_qutex) {
      so_far_so_good = h_core_bool_false;
      h_core_trace("h_core_qutex_create");
    }
  }

  if (!so_far_so_good) {
    if (cache->objects) {
      h_container_set_destroy(cache->objects);
    }
    if (cache->objects_qutex) {
      h_core_qutex_destroy(cache->objects_qutex);
    }
    free(cache);
    cache = NULL;
  }

  return cache;
}

void h_container_cache_destroy(h_container_cache_t *cache)
{
  assert(cache);
  h_container_set_destroy(cache->objects);
  h_core_qutex_destroy(cache->objects_qutex);
}

void *h_container_cache_find(h_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *found_object;

  h_core_qutex_lock_shared(cache->objects_qutex);
  found_object = h_container_set_find(cache->objects, object);
  h_core_qutex_unlock_shared(cache->objects_qutex);

  return found_object;
}

void *h_container_cache_find_copy(h_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *object_copy;

  h_core_qutex_lock_shared(cache->objects_qutex);
  object_copy = h_container_set_find_copy(cache->objects, object);
  h_core_qutex_unlock_shared(cache->objects_qutex);

  return object_copy;
}

unsigned long h_container_cache_get_size(h_container_cache_t *cache)
{
  assert(cache);
  unsigned long size;

  h_core_qutex_lock_shared(cache->objects_qutex);
  size = h_container_set_get_size(cache->objects);
  h_core_qutex_unlock_shared(cache->objects_qutex);

  return size;
}

void h_container_cache_remove_objects(h_container_cache_t *cache)
{
  assert(cache);
  void *object;

  h_core_qutex_lock_shared(cache->objects_qutex);
  {
    h_container_set_iterate_start(cache->objects);
    while ((object = h_container_set_iterate_next(cache->objects))) {
      if (cache->remove_condition(object)) {
        h_container_set_iterate_remove(cache->objects);
      }
    }
  }
  h_core_qutex_unlock_shared(cache->objects_qutex);
}
