#include "h/core/tools.h"
#include "h/core/void.h"

int h_core_void_compare(void *object_a, void *object_b)
{
  int compare;

  if (object_a > object_b) {
    compare = 1;
  } else if (object_a < object_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *h_core_void_copy(void *object)
{
  return object;
}

void h_core_void_destroy(void *object)
{
  free(object);
}

char *h_core_void_get_as_string(void *object)
{
  h_core_trace_exit("TODO: implement/test");

  assert(object);
  char *string;

  string = malloc(10 + 1);
  if (string) {
    snprintf(string, 10 + 1, "%p", object);
  } else {
    h_core_trace_exit("malloc");
  }

  return string;
}

void h_core_void_init_objectey(h_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = h_core_void_get_as_string;
  objectey->compare = h_core_void_compare;
  objectey->copy = h_core_void_copy;
  objectey->destroy = h_core_void_destroy;
}
