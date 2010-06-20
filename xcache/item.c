#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/xcache/item.h"

struct h_xcache_item_t {
  time_t birth_time;
  unsigned long lifespan_seconds;
  h_core_bool_t expired;
  h_core_uuid_t *object_uuid;
  void *object;
  h_core_copy_f copy;
  h_core_destroy_f destroy;
};

int h_xcache_item_compare(void *item_object_a,
    void *item_object_b)
{
  assert(item_object_a);
  assert(item_object_b);
  h_xcache_item_t *item_a;
  h_xcache_item_t *item_b;

  item_a = item_object_a;
  item_b = item_object_b;

  return h_core_uuid_compare(item_a->object_uuid, item_b->object_uuid);
}

void *h_xcache_item_copy(void *item_object)
{
  h_core_trace_exit("TODO: implement");
}

h_xcache_item_t *h_xcache_item_create(h_core_uuid_t *object_uuid,
    void *object, h_core_copy_f copy,
    h_core_destroy_f destroy, unsigned long lifespan_seconds)
{
  assert(object_uuid);
  assert(object);
  assert(copy);
  h_xcache_item_t *item;

  item = malloc(sizeof *item);
  if (item) {
    item->birth_time = time(NULL);
    item->object = object;
    item->copy = copy;
    item->destroy = destroy;
    item->lifespan_seconds = lifespan_seconds;
    item->expired = h_core_bool_false;
    item->object_uuid = h_core_uuid_copy(object_uuid);
    if (!item->object_uuid) {
      h_core_trace("h_core_uuid_copy");
      free(item);
      item = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return item;
}

h_xcache_item_t *h_xcache_item_create_decoy(h_core_uuid_t *object_uuid)
{
  assert(object_uuid);
  h_xcache_item_t *item;

  item = malloc(sizeof *item);
  if (item) {
    item->object_uuid = h_core_uuid_copy(object_uuid);
    if (!item->object_uuid) {
      h_core_trace("h_core_uuid_copy");
      free(item);
      item = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return item;
}

void h_xcache_item_destroy(void *item_object)
{
  assert(item_object);
  h_xcache_item_t *item;

  item = item_object;

  h_core_uuid_destroy(item->object_uuid);
  if (item->destroy) {
    item->destroy(item->object);
  }
  free(item);
}

void h_xcache_item_destroy_decoy(void *item_object)
{
  assert(item_object);
  h_xcache_item_t *item;

  item = item_object;

  h_core_uuid_destroy(item->object_uuid);
  free(item);
}

void h_xcache_item_expire(h_xcache_item_t *item)
{
  item->expired = h_core_bool_true;
}

void *h_xcache_item_get_object(h_xcache_item_t *item)
{
  return item->object;
}

void *h_xcache_item_get_object_copy(h_xcache_item_t *item)
{
  return item->copy(item->object);
}

h_core_uuid_t *h_xcache_item_get_object_uuid(h_xcache_item_t *item)
{
  return item->object_uuid;
}

h_core_bool_t h_xcache_item_is_expired(void *item_object)
{
  assert(item_object);
  h_xcache_item_t *item;
  h_core_bool_t is_expired;
  time_t now;

  item = item_object;

  if (item->expired) {
    is_expired = h_core_bool_true;
  } else {
    now = time(NULL);
    if ((now - item->birth_time) > item->lifespan_seconds) {
      is_expired = h_core_bool_true;
    } else {
      is_expired = h_core_bool_false;
    }
  }

  return is_expired;
}
