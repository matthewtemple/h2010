#ifndef h_container_list_h
#define h_container_list_h

#include "h/container/declarations.h"
#include "h/core/message.h"
#include "h/core/object.h"
#include "h/core/objectey.h"
#include "h/core/types.h"

#define H_CONTAINER_LIST_NO_SIZE_LIMIT 0

struct h_container_list_t;
typedef struct h_container_list_t h_container_list_t;

h_core_bool_t h_container_list_absorb(h_container_list_t *list,
    h_container_list_t *absorb_these);

h_core_bool_t h_container_list_add_to_message(h_container_list_t *list,
    h_core_message_t *message, h_core_message_add_to_message_f add_to_message);

h_core_bool_t h_container_list_add_first(h_container_list_t *list,
    void *object);

h_core_bool_t h_container_list_add_last(h_container_list_t *list,
    void *object);

void h_container_list_clear(h_container_list_t *list);

int h_container_list_compare(void *list_object_a, void *list_object_b);

void *h_container_list_copy(void *list_object);

h_container_list_t *h_container_list_create(h_core_compare_f compare,
    h_core_copy_f copy, h_core_destroy_f destroy);

h_container_list_t *h_container_list_create_from_array_n
(h_container_array_t *array, unsigned long max_size);

h_container_list_t *h_container_list_create_from_message
(h_core_compare_f compare, h_core_copy_f copy, h_core_destroy_f destroy,
    h_core_message_t *message,
    h_core_message_create_from_message_f create_from_message);

h_container_list_t *h_container_list_create_strings_from_string(char *string,
    const char *separators);

void h_container_list_destroy(void *list_object);

void h_container_list_dont_destroys(h_container_list_t *list);

void *h_container_list_find_at(h_container_list_t *list, unsigned long index);

void *h_container_list_find_first(h_container_list_t *list);

void *h_container_list_find_last(h_container_list_t *list);

void *h_container_list_find_random(h_container_list_t *list);

char *h_container_list_get_as_delimited_string(h_container_list_t *list,
    h_core_get_as_string_f get_as_string, const char *delimiter);

char *h_container_list_get_as_string(void *list_object);

unsigned long h_container_list_get_size(h_container_list_t *list);

void h_container_list_init_objectey(h_core_objectey_t *objectey);

void *h_container_list_iterate_next(h_container_list_t *list);

void h_container_list_iterate_remove(h_container_list_t *list);

void h_container_list_iterate_start(h_container_list_t *list);

void h_container_list_lock(h_container_list_t *list);

void h_container_list_print(void *list_object);

h_core_bool_t h_container_list_remove_first(h_container_list_t *list);

h_core_bool_t h_container_list_remove_last(h_container_list_t *list);

void h_container_list_set_size_limit(h_container_list_t *list,
    unsigned long size_limit);

void h_container_list_unlock(h_container_list_t *list);

#endif
