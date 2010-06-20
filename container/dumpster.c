#include "h/container/dumpster.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_container_dumpster_t {
  h_core_objectey_t *objectey;
  h_container_list_t *objects;
};

h_core_bool_t h_container_dumpster_add(h_container_dumpster_t *dumpster,
    void *object)
{
  assert(dumpster);
  assert(object);
  h_core_bool_t success;

  if (h_container_list_add_last(dumpster->objects, object)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

h_container_dumpster_t *h_container_dumpster_create
(h_core_objectey_t *objectey)
{
  assert(objectey);
  h_container_dumpster_t *dumpster;

  dumpster = malloc(sizeof *dumpster);
  if (dumpster) {
    dumpster->objectey = objectey;
    dumpster->objects = h_container_list_create(objectey->compare,
        objectey->copy, objectey->destroy);
    if (!dumpster->objects) {
      free(dumpster);
      dumpster = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return dumpster;
}

void h_container_dumpster_destroy(h_container_dumpster_t *dumpster)
{
  assert(dumpster);

  h_container_list_destroy(dumpster->objects);
  free(dumpster);
}

h_core_bool_t h_container_dumpster_take_objects_from_list
(h_container_dumpster_t *dumpster, h_container_list_t *list)
{
  assert(dumpster);
  assert(list);
  h_core_bool_t success;
  void *object;

  success = h_core_bool_true;

  h_container_list_dont_destroys(list);
  h_container_list_iterate_start(list);
  while ((object = h_container_list_iterate_next(list))) {
    if (h_container_dumpster_add(dumpster, object)) {
      h_container_list_iterate_remove(list);
    } else {
      success = h_core_bool_false;
      h_core_trace("h_container_dumpster_add_object");
      break;
    }
  }

  return success;
}
