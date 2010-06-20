#ifndef h_core_long_h
#define h_core_long_h

#include "h/core/object.h"
#include "h/core/objectey.h"
#include "h/core/types.h"

h_core_bool_t h_core_long_add_to_message(void *long_object,
    h_core_message_t *message);

int h_core_long_compare(void *long_object_a, void *long_object_b);

void *h_core_long_copy(void *long_object);

void *h_core_long_create_from_message(h_core_message_t *message);

void h_core_long_destroy(void *long_object);

char *h_core_long_get_as_string(void *long_object);

void h_core_long_init_objectey(h_core_objectey_t *objectey);

void h_core_long_print(void *long_object);

#endif
