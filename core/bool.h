#ifndef h_core_bool_h
#define h_core_bool_h

#include "h/core/declarations.h"

enum h_core_bool_t {
  h_core_bool_false = 0,
  h_core_bool_true = 1,
};
typedef enum h_core_bool_t h_core_bool_t;

h_core_bool_t h_core_bool_add_to_message(void *bool_object,
    h_core_message_t *message);

void *h_core_bool_create_from_message(h_core_message_t *message);

#endif
