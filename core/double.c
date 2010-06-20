#include "h/core/double.h"
#include "h/core/message.h"
#include "h/core/tools.h"
#include "h/external/external.h"

h_core_bool_t h_core_double_add_to_message(void *double_object,
    h_core_message_t *message)
{
  return h_core_message_add_double(message, double_object);
}

int h_core_double_compare(void *double_object_a,
    void *double_object_b)
{
  assert(double_object_a);
  assert(double_object_b);
  double *double_a;
  double *double_b;
  int compare_result;

  double_a = double_object_a;
  double_b = double_object_b;

  if (double_a < double_b) {
    compare_result = -1;
  } else if (double_a > double_b) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void *h_core_double_copy(void *double_object)
{
  assert(double_object);
  double *double_value;
  double *double_copy;

  double_value = double_object;
  double_copy = malloc(sizeof *double_copy);
  if (double_copy) {
    *double_copy = *double_value;
  } else {
    h_core_trace("malloc");
  }

  return double_copy;
}

void *h_core_double_create_from_message(h_core_message_t *message)
{
  return h_core_message_take_double(message);
}

void h_core_double_destroy(void *double_object)
{
  free(double_object);
}
