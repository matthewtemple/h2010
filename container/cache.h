#ifndef h_container_cache_h
#define h_container_cache_h

#include "h/core/object.h"
#include "h/core/qutex.h"

struct h_container_cache_t;
typedef struct h_container_cache_t h_container_cache_t;

h_core_bool_t h_container_cache_add(h_container_cache_t *cache, void *object);

void h_container_cache_clear(h_container_cache_t *cache);

h_container_cache_t *h_container_cache_create(h_core_compare_f compare,
    h_core_copy_f copy, h_core_destroy_f destroy,
    h_core_condition_f remove_condition);

void h_container_cache_destroy(h_container_cache_t *cache);

void *h_container_cache_find(h_container_cache_t *cache, void *object);

void *h_container_cache_find_copy(h_container_cache_t *cache, void *object);

unsigned long h_container_cache_get_size(h_container_cache_t *cache);

void h_container_cache_remove_objects(h_container_cache_t *cache);

#endif
