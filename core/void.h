#ifndef h_net_void_h
#define h_net_void_h

#include "h/core/objectey.h"
#include "h/external/external.h"

int h_core_void_compare(void *object_a, void *object_b);

void *h_core_void_copy(void *object);

void h_core_void_destroy(void *object);

char *h_core_void_get_as_string(void *object);

void h_core_void_init_objectey(h_core_objectey_t *objectey);

#endif
