#ifndef h_core_uuid_h
#define h_core_uuid_h

#include "h/core/bool.h"
#include "h/core/message.h"
#include "h/core/object.h"

struct h_core_uuid_t;
typedef struct h_core_uuid_t h_core_uuid_t;

h_core_bool_t h_core_uuid_add_to_message(void *uuid_object,
    h_core_message_t *message);

int h_core_uuid_compare(void *uuid_object_a, void *uuid_object_b);

void *h_core_uuid_copy(void *uuid_object);

h_core_uuid_t *h_core_uuid_create();

void *h_core_uuid_create_from_message(h_core_message_t *message);

h_core_uuid_t *h_core_uuid_create_from_string(const char *uuid_string);

h_core_uuid_t *h_core_uuid_create_null();

void h_core_uuid_destroy(void *uuid_object);

unsigned long h_core_uuid_get_memory_size_bytes(h_core_uuid_t *uuid);

char *h_core_uuid_get_string(void *uuid_object);

unsigned long h_core_uuid_hash(h_core_uuid_t *uuid);

h_core_bool_t h_core_uuid_is_null(h_core_uuid_t *uuid);

#endif
