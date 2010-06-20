#ifndef h_core_string_h
#define h_core_string_h

#include "h/core/object.h"
#include "h/core/objectey.h"
#include "h/core/types.h"

typedef char * h_core_string_t;

h_core_bool_t h_core_string_add_to_message(void *string_object,
    h_core_message_t *message);

int h_core_string_compare(void *string_object_a, void *string_object_b);

void *h_core_string_copy(void *string_object);

void *h_core_string_create_from_message(h_core_message_t *message);

void h_core_string_destroy(void *string_object);

char *h_core_string_get_as_string(void *string_object);

unsigned long h_core_string_hash(void *string_object);

void h_core_string_init_objectey(h_core_objectey_t *objectey);

void h_core_string_print(void *string_object);

h_core_string_t h_core_string_substring(h_core_string_t string,
    unsigned long start, unsigned long length);

#endif
