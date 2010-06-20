#include "h/container/set.h"
#include "h/container/shardset.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_container_shardset_t {
  h_core_hash_f hash_object;
  unsigned short shard_count;
  h_container_set_t *shards[H_CONTAINER_SHARDSET_MAX_SHARDS];
  pthread_mutex_t shard_mutexes[H_CONTAINER_SHARDSET_MAX_SHARDS];

  h_core_bool_t iterate_remove;
  h_core_bool_t iterate_first;
  unsigned short iterator_shard;
  void *iterator;

  pthread_mutex_t mutex;
};

static void h_container_shardset_create_rollback
(h_container_shardset_t *shardset);

static unsigned short get_shard_id_for_object(h_container_shardset_t *shardset,
    void *object);

h_core_bool_t h_container_shardset_add(h_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  h_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success = h_container_set_add(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

h_core_bool_t h_container_shardset_add_replace
(h_container_shardset_t *shardset, void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  h_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success
    = h_container_set_add_replace(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

void h_container_shardset_clear(h_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    h_container_set_clear(*(shardset->shards + each_shard));
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }
}

/*
  TODO: simplify
  TODO: ugh...some rollback code is in the rollback method, some not...fix
*/
h_container_shardset_t *h_container_shardset_create
(h_core_compare_f compare, h_core_copy_f copy,
    h_core_destroy_f destroy, h_core_hash_f hash_object,
    unsigned short shard_count)
{
  assert(shard_count > 0);
  h_container_shardset_t *shardset;
  h_core_bool_t so_far_so_good;
  unsigned short each_shard;
  h_core_bool_t mutex_needs_destroy;

  mutex_needs_destroy = h_core_bool_false;

  if (shard_count > H_CONTAINER_SHARDSET_MAX_SHARDS) {
    shard_count = H_CONTAINER_SHARDSET_MAX_SHARDS;
  }

  shardset = malloc(sizeof *shardset);
  if (shardset) {
    shardset->shard_count = shard_count;
    shardset->hash_object = hash_object;
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      *(shardset->shards + each_shard) = NULL;
    }
    so_far_so_good = h_core_bool_true;
  } else {
    h_core_trace("malloc");
    so_far_so_good = h_core_bool_false;
  }

  if (so_far_so_good) {
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      *(shardset->shards + each_shard) = h_container_set_create
        (compare, copy, destroy);
      if (!*(shardset->shards + each_shard)) {
        h_core_trace("h_container_set_create");
        so_far_so_good = h_core_bool_false;
        break;
      }
    }
  }

  if (so_far_so_good) {
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      if (0 != pthread_mutex_init
          (shardset->shard_mutexes + each_shard, NULL)) {
        h_core_trace("pthread_mutex_init");
        so_far_so_good = h_core_bool_false;
        break;
      }
    }
  }

  if (so_far_so_good) {
    if (0 == pthread_mutex_init(&shardset->mutex, NULL)) {
      mutex_needs_destroy = h_core_bool_true;
    } else {
      h_core_trace("pthread_mutex_init");
    }
  }

  if (!so_far_so_good && shardset) {
    h_container_shardset_create_rollback(shardset);
    if (mutex_needs_destroy) {
      if (!pthread_mutex_destroy(&shardset->mutex)) {
        h_core_trace("pthread_mutex_destroy");
      }
    }
    free(shardset);
    shardset = NULL;
  }

  return shardset;
}

void h_container_shardset_create_rollback(h_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  if (shardset->shards) {
    for (each_shard = 0; each_shard < shardset->shard_count;
         each_shard++) {
      if (*(shardset->shards + each_shard)) {
        h_container_set_destroy(*(shardset->shards + each_shard));
      }
    }
    free(shardset->shards);
  }

  if (shardset->shard_mutexes) {
    for (each_shard = 0; each_shard < shardset->shard_count;
         each_shard++) {
      if (0 != pthread_mutex_destroy
          (shardset->shard_mutexes + each_shard)) {
        h_core_trace("pthread_mutex_destroy");
      }
    }
  }
}

void h_container_shardset_destroy(h_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    h_container_set_destroy(*(shardset->shards + each_shard));
    if (0 != pthread_mutex_destroy(shardset->shard_mutexes + each_shard)) {
      h_core_trace("pthread_mutex_destroy");
    }
  }

  if (0 != pthread_mutex_destroy(&shardset->mutex)) {
    h_core_trace("pthread_mutex_destroy");
  }

  free(shardset);
}

void *h_container_shardset_find(h_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  void *found_object;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  found_object = h_container_set_find(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return found_object;
}

void *h_container_shardset_find_copy
(h_container_shardset_t *shardset, void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  void *found_object;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  found_object = h_container_set_find_copy
    (*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return found_object;
}

unsigned long h_container_shardset_get_size(h_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;
  unsigned long total_size;

  total_size = 0;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    total_size += h_container_set_get_size(*(shardset->shards + each_shard));
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }

  return total_size;
}

void *h_container_shardset_iterate_next
(h_container_shardset_t *shardset)
{
  assert(shardset);
  h_container_set_t *set;
  void *next_object;

  if (shardset->iterator) {
    if (shardset->iterate_first) {
      shardset->iterate_first = h_core_bool_false;
      next_object = shardset->iterator;
    } else {
      set = shardset->shards[shardset->iterator_shard];
      if (shardset->iterate_remove) {
        h_container_set_iterate_remove(set);
        shardset->iterate_remove = h_core_bool_false;
      }
      shardset->iterator = h_container_set_iterate_next(set);
      next_object = shardset->iterator;
    }
  } else {
    next_object = NULL;
  }

  /*
    TODO: there may be a cleaner implementation...this block (almost) is
    repeated in h_container_shardset_iterate_start().  this implementation is
    sortof a hybrid of the way h_core_set/list/array do iteration, crossed with
    the changes necessary in this context.  it may be that in this context
    there is an implementation with much less happening in
    h_container_shardset_iterate_start().
  */
  while (!shardset->iterator) {
    pthread_mutex_unlock(shardset->shard_mutexes + shardset->iterator_shard);
    if (shardset->iterator_shard < (shardset->shard_count - 1)) {
      shardset->iterator_shard++;
      pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
      set = shardset->shards[shardset->iterator_shard];
      h_container_set_iterate_start(set);
      shardset->iterator = h_container_set_iterate_next(set);
      next_object = shardset->iterator;
    } else {
      break;
    }
  }

  return next_object;
}

void h_container_shardset_iterate_remove(h_container_shardset_t *shardset)
{
  shardset->iterate_remove = h_core_bool_true;
}

void h_container_shardset_iterate_start(h_container_shardset_t *shardset)
{
  assert(shardset);
  h_container_set_t *set;

  shardset->iterator_shard = 0;
  set = shardset->shards[shardset->iterator_shard];

  pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
  h_container_set_iterate_start(set);
  shardset->iterator = h_container_set_iterate_next(set);
  while (!shardset->iterator) {
    pthread_mutex_unlock(shardset->shard_mutexes + shardset->iterator_shard);
    if (shardset->iterator_shard < (shardset->shard_count - 1)) {
      shardset->iterator_shard++;
      pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
      set = shardset->shards[shardset->iterator_shard];
      h_container_set_iterate_start(set);
      shardset->iterator = h_container_set_iterate_next(set);
    } else {
      break;
    }
  }

  shardset->iterate_remove = h_core_bool_false;
  shardset->iterate_first = h_core_bool_true;
}

void h_container_shardset_lock(h_container_shardset_t *shardset)
{
  pthread_mutex_lock(&shardset->mutex);
}

void h_container_shardset_print(h_container_shardset_t *shardset,
    h_core_get_as_string_f get_object_as_string)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    h_container_set_print(*(shardset->shards + each_shard),
        get_object_as_string);
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }
}

h_core_bool_t h_container_shardset_remove(h_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  h_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success = h_container_set_remove(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

unsigned long h_container_shardset_remove_if(h_container_shardset_t *shardset,
    h_core_condition_f object_condition)
{
  assert(shardset);
  unsigned short each_shard;
  h_container_set_t *set;
  void *object;
  unsigned long total_items_removed;

  total_items_removed = 0;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {

    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    {
      set = *(shardset->shards + each_shard);
      h_container_set_iterate_start(set);
      while ((object = h_container_set_iterate_next(set))) {
        if (object_condition(object)) {
          h_container_set_iterate_remove(set);
          total_items_removed++;
        }
      }
    }
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);

  }

  return total_items_removed;
}

void h_container_shardset_unlock(h_container_shardset_t *shardset)
{
  pthread_mutex_unlock(&shardset->mutex);
}

unsigned short get_shard_id_for_object(h_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned long hash;
  unsigned short id;

  hash = shardset->hash_object(object);
  id = hash % shardset->shard_count;

  return id;
}
