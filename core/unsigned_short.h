#ifndef h_core_unsigned_short_h
#define h_core_unsigned_short_h

#include "h/core/object.h"
#include "h/core/types.h"

int h_core_unsigned_short_compare(void *unsigned_short_object_a,
    void *unsigned_short_object_b);

void *h_core_unsigned_short_copy(void *unsigned_short_object);

void h_core_unsigned_short_destroy(void *unsigned_short_object);

void h_core_unsigned_short_print(void *unsigned_short_object);

#endif
