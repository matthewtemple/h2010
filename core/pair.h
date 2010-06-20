#ifndef h_core_pair_h
#define h_core_pair_h

#include "h/core/object.h"
#include "h/core/objectey.h"

#define H_CORE_PAIR_DESTROYS h_core_bool_true
#define H_CORE_PAIR_DONT_DESTROY h_core_bool_false

struct h_core_pair_t;
typedef struct h_core_pair_t h_core_pair_t;

int h_core_pair_compare(void *pair_object_a, void *pair_object_b);

int h_core_pair_compare_left(void *pair_object_a, void *pair_object_b);

int h_core_pair_compare_right(void *pair_object_a, void *pair_object_b);

void *h_core_pair_copy(void *pair_object);

h_core_pair_t *h_core_pair_create(void *left, h_core_objectey_t *left_objectey,
    void *right, h_core_objectey_t *right_objectey, h_core_bool_t destroys);

h_core_pair_t *h_core_pair_create_decoy(void *left,
    h_core_objectey_t *left_objectey);

void h_core_pair_destroy(void *pair_object);

void h_core_pair_destroy_decoy(h_core_pair_t *pair);

char *h_core_pair_get_as_string(void *pair_object);

void *h_core_pair_get_left(h_core_pair_t *pair);

h_core_objectey_t *h_core_pair_get_left_objectey(h_core_pair_t *pair);

void *h_core_pair_get_right(h_core_pair_t *pair);

h_core_objectey_t *h_core_pair_get_right_objectey(h_core_pair_t *pair);

void h_core_pair_init_objectey(h_core_objectey_t *objectey);

#endif
