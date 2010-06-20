#ifndef h_xcache_system_h
#define h_xcache_system_h

#include "h/core/object.h"
#include "h/core/uuid.h"

struct h_xcache_system_t;
typedef struct h_xcache_system_t h_xcache_system_t;

h_core_bool_t h_xcache_system_add(h_xcache_system_t *system,
    h_core_uuid_t *object_uuid, void *object,
    h_core_copy_f copy, h_core_destroy_f destroy,
    unsigned long lifetime_seconds);

void h_xcache_system_clear(h_xcache_system_t *system);

h_xcache_system_t *h_xcache_system_create();

void h_xcache_system_destroy(h_xcache_system_t *system);

void *h_xcache_system_find_wait_copy_expire(h_xcache_system_t *system,
    h_core_uuid_t *object_uuid, unsigned long max_wait_seconds);

unsigned long h_xcache_system_get_size(h_xcache_system_t *system);

void h_xcache_system_remove_objects(h_xcache_system_t *system);

#endif
