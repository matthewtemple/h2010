#include "h/core/long.h"
#include "h/core/message.h"
#include "h/core/tools.h"
#include "h/external/external.h"

h_core_bool_t h_core_long_add_to_message(void *long_object,
    h_core_message_t *message)
{
  return h_core_message_add_long(message, long_object);
}

int h_core_long_compare(void *long_object_a,
    void *long_object_b)
{
  long *long_a;
  long *long_b;
  int compare;

  long_a = long_object_a;
  long_b = long_object_b;

  if (*long_a > *long_b) {
    compare = 1;
  } else if (*long_a < *long_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *h_core_long_copy(void *long_object)
{
  assert(long_object);
  long *long_value;
  long *long_copy;

  long_value = long_object;
  long_copy = malloc(sizeof *long_copy);
  if (long_copy) {
    *long_copy = *long_value;
  } else {
    h_core_trace("malloc");
  }

  return long_copy;
}

void *h_core_long_create_from_message(h_core_message_t *message)
{
  return h_core_message_take_long(message);
}

void h_core_long_destroy(void *long_object)
{
  assert(long_object);
  free(long_object);
}

char *h_core_long_get_as_string(void *long_object)
{
  assert(long_object);
  long *long_value;
  char *string;

  long_value = long_object;

  if (-1 == asprintf(&string, "%li", *long_value)) {
    string = NULL;
  }

  return string;
}

void h_core_long_init_objectey(h_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = h_core_long_get_as_string;
  objectey->compare = h_core_long_compare;
  objectey->copy = h_core_long_copy;
  objectey->destroy = h_core_long_destroy;
}

void h_core_long_print(void *long_object)
{
  h_core_print(long_object, h_core_long_get_as_string);
}
