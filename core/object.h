#ifndef h_core_object_h
#define h_core_object_h

#include "h/core/bool.h"

#define H_CORE_NO_COMPARE_FUNCTION NULL
#define H_CORE_NO_COPY_FUNCTION NULL
#define H_CORE_NO_DESTROY_FUNCTION NULL

typedef int (*h_core_compare_f)(void *a_object, void *b_object);

typedef int (*h_core_compare_const_f)(const void *a_object,
    const void *b_object);

typedef h_core_bool_t (*h_core_condition_f)(void *object);

typedef void *(*h_core_copy_f)(void *object);

typedef void *(*h_core_create_f)();

typedef void (*h_core_destroy_f)(void *object);

typedef char *(*h_core_get_as_string_f)(void *object);

typedef void *(*h_core_get_object_f)(void *object);

typedef unsigned long (*h_core_hash_f)(void *object);

void h_core_print(void *object, h_core_get_as_string_f get_as_string);

typedef void (*h_core_set_object_f)(void *object, void *value);

#endif
