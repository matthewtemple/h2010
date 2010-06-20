#ifndef h_container_array_h
#define h_container_array_h

#include "h/container/declarations.h"
#include "h/core/bool.h"
#include "h/core/message.h"
#include "h/core/object.h"

void h_container_array_add(h_container_array_t *array, unsigned long index,
    void *object);

h_core_bool_t h_container_array_add_to_message(h_container_array_t *array,
    h_core_message_t *message, h_core_message_add_to_message_f add_to_message);

void h_container_array_clear(h_container_array_t *array);

int h_container_array_compare(void *array_object_a, void *array_object_b);

void *h_container_array_copy(void *array_object);

unsigned long h_container_array_count_non_null_objects
(h_container_array_t *array);

h_container_array_t *h_container_array_create(unsigned long initial_size,
    h_core_compare_f compare, h_core_copy_f copy, h_core_destroy_f destroy);

h_container_array_t *h_container_array_create_from_message
(h_core_compare_f compare_object, h_core_copy_f copy,
    h_core_destroy_f destroy, h_core_message_t *message,
    h_core_message_create_from_message_f create_from_message);

h_container_array_t *h_container_array_create_strings_from_string(char *string,
    char *separators);

void h_container_array_destroy(void *array_object);

void h_container_array_expunge_set(h_container_array_t *array,
    h_container_set_t *expunge_these);

void h_container_array_expunge_set_inverse(h_container_array_t *array,
    h_container_set_t *expunge_all_but_these);

void *h_container_array_find(h_container_array_t *array, unsigned long index);

void *h_container_array_find_random(h_container_array_t *array);

void *h_container_array_find_first(h_container_array_t *array);

void *h_container_array_find_last(h_container_array_t *array);

void **h_container_array_get_array(h_container_array_t *array);

char *h_container_array_get_as_delimited_string(h_container_array_t *array,
    h_core_get_as_string_f get_as_string, char *delimiter);

h_core_compare_f h_container_array_get_compare(h_container_array_t *array);

h_core_copy_f h_container_array_get_copy(h_container_array_t *array);

h_core_destroy_f h_container_array_get_destroy(h_container_array_t *array);

unsigned long h_container_array_get_size(h_container_array_t *array);

void *h_container_array_iterate_next(h_container_array_t *array);

void h_container_array_iterate_remove(h_container_array_t *array);

void h_container_array_iterate_start(h_container_array_t *array);

void h_container_array_print(h_container_array_t *array);

void h_container_array_remove(h_container_array_t *array, unsigned long index);

void h_container_array_replace(h_container_array_t *array, unsigned long index,
    void *object);

h_core_bool_t h_container_array_resize(h_container_array_t *array,
    unsigned long new_size);

void h_container_array_sort(h_container_array_t *array);

#endif
