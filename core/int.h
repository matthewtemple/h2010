#ifndef h_core_int_h
#define h_core_int_h

#include "h/core/object.h"
#include "h/core/objectey.h"

struct h_core_int_t;
typedef struct h_core_int_t h_core_int_t;

int h_core_int_compare(void *int_object_a, void *int_object_b);

void *h_core_int_copy(void *int_object);

h_core_int_t *h_core_int_create();

void h_core_int_destroy(void *int_object);

char *h_core_int_get_as_string(void *int_object);

void h_core_int_init_objectey(h_core_objectey_t *objectey);

#endif
