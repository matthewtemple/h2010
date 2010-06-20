#ifndef h_core_objectey_h
#define h_core_objectey_h

#include "h/core/object.h"

struct h_core_objectey_t {
  h_core_compare_f compare;
  h_core_copy_f copy;
  h_core_destroy_f destroy;
  h_core_get_as_string_f get_as_string;
};
typedef struct h_core_objectey_t h_core_objectey_t;

void h_core_objectey_init(h_core_objectey_t *objectey,
    h_core_compare_f compare, h_core_copy_f copy,
    h_core_destroy_f destroy,
    h_core_get_as_string_f get_as_string);

#endif
