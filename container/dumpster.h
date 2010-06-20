#ifndef h_container_dumpster_h
#define h_container_dumpster_h

#include "h/container/array.h"
#include "h/container/list.h"
#include "h/container/set.h"
#include "h/core/objectey.h"

struct h_container_dumpster_t;
typedef struct h_container_dumpster_t h_container_dumpster_t;

h_core_bool_t h_container_dumpster_add(h_container_dumpster_t *dumpster,
    void *object);

h_container_dumpster_t *h_container_dumpster_create
(h_core_objectey_t *objectey);

void h_container_dumpster_destroy(h_container_dumpster_t *dumpster);

h_core_bool_t h_container_dumpster_take_objects_from_list
(h_container_dumpster_t *dumpster, h_container_list_t *list);

#endif
