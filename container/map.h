#ifndef h_container_map_h
#define h_container_map_h

#include "h/core/bool.h"
#include "h/core/object.h"
#include "h/core/objectey.h"

#define H_CONTAINER_MAP_DESTROY h_core_bool_true
#define H_CONTAINER_MAP_DONT_DESTROY h_core_bool_false

struct h_container_map_t;
typedef struct h_container_map_t h_container_map_t;

h_core_bool_t h_container_map_add(h_container_map_t *map, void *left,
    void *right);

int h_container_map_compare(void *map_object_a, void *map_object_b);

void *h_container_map_copy(void *map_object);

h_container_map_t *h_container_map_create(h_core_objectey_t *left_objectey,
    h_core_objectey_t *right_objectey, h_core_bool_t destroys);

void h_container_map_destroy(void *map_object);

void *h_container_map_find(h_container_map_t *map, void *left_object_decoy);

void h_container_map_print(h_container_map_t *map);

h_core_bool_t h_container_map_remove(h_container_map_t *map,
    void *left_object_decoy);

#endif
