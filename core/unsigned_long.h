#ifndef h_core_unsigned_long_h
#define h_core_unsigned_long_h

#include "h/core/objectey.h"

int h_core_unsigned_long_compare(void *unsigned_long_object_a,
    void *unsigned_long_object_b);

void *h_core_unsigned_long_copy(void *unsigned_long_object);

void h_core_unsigned_long_destroy(void *unsigned_long_object);

char *h_core_unsigned_long_get_as_string(void *unsigned_long_object);

void h_core_unsigned_long_init_objectey(h_core_objectey_t *objectey);

#endif
