#include "h/container/map.h"
#include "h/container/set.h"
#include "h/core/pair.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_container_map_t {
  h_container_set_t *pairs;
  h_core_objectey_t *left_objectey;
  h_core_objectey_t *right_objectey;
  h_core_bool_t destroy;
};

h_core_bool_t h_container_map_add(h_container_map_t *map, void *left,
    void *right)
{
  assert(map);
  assert(left);
  assert(right);
  h_core_pair_t *pair;
  h_core_bool_t success;

  pair = h_core_pair_create(left, map->left_objectey, right,
      map->right_objectey, map->destroy);
  if (pair) {
    success = h_core_bool_true;
    h_container_set_add(map->pairs, pair);
  } else {
    success = h_core_bool_false;
    h_core_trace("h_core_pair_create");
  }

  return success;
}

int h_container_map_compare(void *map_object_a, void *map_object_b)
{
  assert(map_object_a);
  assert(map_object_b);
  h_container_map_t *map_a;
  h_container_map_t *map_b;

  map_a = map_object_a;
  map_b = map_object_b;

  return h_container_set_compare(map_a->pairs, map_b->pairs);
}

void *h_container_map_copy(void *map_object)
{
  assert(map_object);
  h_container_map_t *map;
  h_container_map_t *map_copy;

  map = map_object;

  map_copy = h_container_map_create(map->left_objectey, map->right_objectey,
      map->destroy);
  if (map_copy) {
    map_copy->pairs = h_container_set_copy(map->pairs);
    if (!map_copy->pairs) {
      h_core_trace("h_container_set_copy");
      free(map_copy);
      map_copy = NULL;
    }
  } else {
    h_core_trace("h_container_map_create");
  }

  return map_copy;
}

h_container_map_t *h_container_map_create(h_core_objectey_t *left_objectey,
    h_core_objectey_t *right_objectey, h_core_bool_t destroy)
{
  assert(left_objectey);
  assert(right_objectey);
  h_container_map_t *map;

  map = malloc(sizeof *map);
  if (map) {
    map->left_objectey = left_objectey;
    map->right_objectey = right_objectey;
    map->destroy = destroy;
    map->pairs = h_container_set_create(h_core_pair_compare_left,
        h_core_pair_copy, h_core_pair_destroy);
    if (!map->pairs) {
      h_core_trace("h_container_set_create");
      free(map);
      map = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return map;
}

void h_container_map_destroy(void *map_object)
{
  assert(map_object);
  h_container_map_t *map;

  map = map_object;

  h_container_set_destroy(map->pairs);
  free(map);
}

void *h_container_map_find(h_container_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  h_core_pair_t *found_pair;
  void *found_object;
  h_core_pair_t *pair_decoy;

  pair_decoy = h_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    found_pair = h_container_set_find(map->pairs, pair_decoy);
    h_core_pair_destroy_decoy(pair_decoy);
    if (found_pair) {
      found_object = h_core_pair_get_right(found_pair);
    } else {
      found_object = NULL;
    }
  } else {
    found_object = NULL;
    h_core_trace("pair_create_decoy");
  }

  return found_object;
}

void h_container_map_print(h_container_map_t *map)
{
  h_container_set_print(map->pairs, h_core_pair_get_as_string);
}

h_core_bool_t h_container_map_remove(h_container_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  h_core_bool_t success;
  h_core_pair_t *pair_decoy;

  pair_decoy = h_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    success = h_container_set_remove(map->pairs, pair_decoy);
  } else {
    success = h_core_bool_false;
    h_core_trace("pair_create_decoy");
  }

  return success;
}
