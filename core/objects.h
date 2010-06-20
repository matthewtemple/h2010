#ifndef h_core_objects_h
#define h_core_objects_h

#include "h/core/objectey.h"

struct h_core_objects_t {
  h_core_objectey_t long_objectey;
  h_core_objectey_t pair_objectey;
  h_core_objectey_t string_objectey;
  h_core_objectey_t unsigned_long_objectey;
  h_core_objectey_t void_objectey;
};
typedef struct h_core_objects_t h_core_objects_t;

void h_core_objects_free(h_core_objects_t *objects);

void h_core_objects_init(h_core_objects_t *objects);

#endif
