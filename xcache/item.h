#ifndef h_xcache_item_h
#define h_xcache_item_h

#include "h/core/object.h"
#include "h/core/uuid.h"

struct h_xcache_item_t;
typedef struct h_xcache_item_t h_xcache_item_t;

int h_xcache_item_compare(void *item_object_a, void *item_object_b);

void *h_xcache_item_copy(void *item_object);

h_xcache_item_t *h_xcache_item_create(h_core_uuid_t *object_uuid,
    void *object, h_core_copy_f copy,
    h_core_destroy_f destroy, unsigned long lifespan_seconds);

h_xcache_item_t *h_xcache_item_create_decoy(h_core_uuid_t *object_uuid);

void h_xcache_item_destroy(void *item_object);

void h_xcache_item_destroy_decoy(void *item_object);

void h_xcache_item_expire(h_xcache_item_t *item);

void *h_xcache_item_get_object(h_xcache_item_t *item);

void *h_xcache_item_get_object_copy(h_xcache_item_t *item);

h_core_uuid_t *h_xcache_item_get_object_uuid(h_xcache_item_t *item);

h_core_bool_t h_xcache_item_is_expired(void *item_object);

#endif
