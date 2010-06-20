#ifndef h_core_short_h
#define h_core_short_h

#include "h/core/object.h"
#include "h/core/types.h"

h_core_bool_t h_core_short_add_to_message(void *short_object,
    h_core_message_t *message);

int h_core_short_compare(void *short_object_a, void *short_object_b);

void *h_core_short_copy(void *short_object);

void *h_core_short_take_from_message(h_core_message_t *message);

void h_core_short_destroy(void *short_object);

void h_core_short_print(void *short_object);

#endif
