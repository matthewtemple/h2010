#ifndef h_core_nameobject_h
#define h_core_nameobject_h

#include "h/core/object.h"

struct h_core_nameobject_t;
typedef struct h_core_nameobject_t h_core_nameobject_t;

int h_core_nameobject_compare(void *nameobject_object_a,
    void *nameobject_object_b);

void *h_core_nameobject_copy(void *nameobject_object);

h_core_nameobject_t *h_core_nameobject_create(char *name, void *object,
    h_core_copy_f copy, h_core_destroy_f destroy,
    h_core_get_as_string_f get_as_string);

h_core_nameobject_t *h_core_nameobject_create_decoy(char *name);

void h_core_nameobject_destroy(void *nameobject_object);

void h_core_nameobject_destroy_decoy(void *nameobject_object);

char *h_core_nameget_as_string(void *nameobject_object);

char *h_core_nameobject_get_name(h_core_nameobject_t *nameobject);

void *h_core_nameobject_get_object(h_core_nameobject_t *nameobject);

void h_core_nameobject_print(void *nameobject_object);

#endif
