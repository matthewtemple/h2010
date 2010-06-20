#include "h/core/bool.h"
#include "h/core/message.h"

h_core_bool_t h_core_bool_add_to_message(void *bool_object,
    h_core_message_t *message)
{
  return h_core_message_add_bool(message, bool_object);
}

void *h_core_bool_create_from_message(h_core_message_t *message)
{
  return h_core_message_take_bool(message);
}
