#ifndef h_container_set_h
#define h_container_set_h

#include "h/container/list.h"
#include "h/core/declarations.h"
#include "h/core/message.h"
#include "h/core/object.h"
#include "h/core/types.h"

h_core_bool_t h_container_set_absorb(h_container_set_t *set,
    h_container_set_t *absorb_these);

h_core_bool_t h_container_set_absorb_array(h_container_set_t *set,
    h_container_array_t *absorb_these);

h_core_bool_t h_container_set_absorb_copy(h_container_set_t *set,
    h_container_set_t *absorb_these);

h_core_bool_t h_container_set_absorb_list_copy(h_container_set_t *set,
    h_container_list_t *absorb_these);

h_core_bool_t h_container_set_add(h_container_set_t *set, void *object);

h_core_bool_t h_container_set_add_replace(h_container_set_t *set,
    void *object);

h_core_bool_t h_container_set_add_to_message(h_container_set_t *set,
    h_core_message_t *message, h_core_message_add_to_message_f add_to_message);

void h_container_set_clear(h_container_set_t *set);

int h_container_set_compare(void *set_object_a, void *set_object_b);

h_core_bool_t h_container_set_contains(h_container_set_t *superset,
    h_container_set_t *subset);

void *h_container_set_copy(void *set_object);

h_container_set_t *h_container_set_create(h_core_compare_f compare,
    h_core_copy_f copy, h_core_destroy_f destroy);

h_container_set_t *h_container_set_create_from_message
(h_core_compare_f compare, h_core_copy_f copy, h_core_destroy_f destroy,
    h_core_message_t *message,
    h_core_message_create_from_message_f create_from_message);

h_container_set_t *h_container_set_create_from_union(h_container_set_t *set_a,
    h_container_set_t *set_b);

void h_container_set_destroy(void *set_object);

h_container_set_t *h_container_set_difference(h_container_set_t *set_a,
    h_container_set_t *set_b);

void h_container_set_dont_destroys(h_container_set_t *set);

h_core_bool_t h_container_set_expunge(h_container_set_t *set,
    h_container_set_t *expunge_these);

void *h_container_set_find(h_container_set_t *set, void *decoy_object);

void *h_container_set_find_any(h_container_set_t *set);

void *h_container_set_find_copy(h_container_set_t *set, void *decoy_object);

void *h_container_set_find_take(h_container_set_t *set, void *decoy_object);

h_container_array_t *h_container_set_get_as_array(h_container_set_t *set);

h_container_list_t *h_container_set_get_as_list(h_container_set_t *set);

char *h_container_set_get_as_delimited_string(h_container_set_t *set,
    h_core_get_as_string_f get_as_string, const char *delimiter);

h_core_compare_f h_container_set_get_compare_function(h_container_set_t *set);

h_core_copy_f h_container_set_get_copy_function(h_container_set_t *set);

h_core_destroy_f h_container_set_get_destroy_function(h_container_set_t *set);

unsigned long h_container_set_get_size(h_container_set_t *set);

void *h_container_set_iterate_next(h_container_set_t *set);

void h_container_set_iterate_remove(h_container_set_t *set);

void h_container_set_iterate_start(h_container_set_t *set);

void h_container_set_lock(h_container_set_t *set);

h_core_bool_t h_container_set_overlaps(h_container_set_t *set_a,
    h_container_set_t *set_b);

void h_container_set_print(h_container_set_t *set,
    h_core_get_as_string_f get_as_string);

h_core_bool_t h_container_set_remove(h_container_set_t *set, void *object);

void h_container_set_unlock(h_container_set_t *set);

#endif
