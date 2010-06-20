#ifndef h_core_double_h
#define h_core_double_h

#include "h/core/object.h"

h_core_bool_t h_core_double_add_to_message(void *double_object,
    h_core_message_t *message);

int h_core_double_compare(void *double_object_a, void *double_object_b);

void *h_core_double_copy(void *double_object);

void *h_core_double_create_from_message(h_core_message_t *message);

void h_core_double_destroy(void *double_object);

#endif
