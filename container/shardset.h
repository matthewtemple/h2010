#ifndef h_container_shardset_h
#define h_container_shardset_h

#include "h/core/object.h"
#include "h/core/types.h"

#define H_CONTAINER_SHARDSET_MAX_SHARDS 32768

struct h_container_shardset_t;
typedef struct h_container_shardset_t h_container_shardset_t;

h_core_bool_t h_container_shardset_add(h_container_shardset_t *shardset,
    void *object);

h_core_bool_t h_container_shardset_add_replace
(h_container_shardset_t *shardset, void *object);

void h_container_shardset_clear(h_container_shardset_t *shardset);

h_container_shardset_t *h_container_shardset_create(h_core_compare_f compare,
    h_core_copy_f copy, h_core_destroy_f destroy, h_core_hash_f hash_object,
    unsigned short shard_count);

void h_container_shardset_destroy(h_container_shardset_t *shardset);

void *h_container_shardset_find(h_container_shardset_t *shardset,
    void *object);

void *h_container_shardset_find_copy(h_container_shardset_t *shardset,
    void *object);

unsigned long h_container_shardset_get_size(h_container_shardset_t *shardset);

void *h_container_shardset_iterate_next(h_container_shardset_t *shardset);

void h_container_shardset_lock(h_container_shardset_t *shardset);

void h_container_shardset_iterate_remove(h_container_shardset_t *shardset);

void h_container_shardset_iterate_start(h_container_shardset_t *shardset);

void h_container_shardset_print(h_container_shardset_t *shardset,
    h_core_get_as_string_f get_object_as_string);

h_core_bool_t h_container_shardset_remove(h_container_shardset_t *shardset,
    void *object);

unsigned long h_container_shardset_remove_if(h_container_shardset_t *shardset,
    h_core_condition_f object_condition);

void h_container_shardset_unlock(h_container_shardset_t *shardset);

#endif
