#include "h/core/bit.h"
#include "h/core/tools.h"
#include "h/external/external.h"

int h_core_bit_compare(void *bit_object_a,
    void *bit_object_b)
{
  h_core_bit_t *bit_a;
  h_core_bit_t *bit_b;
  int compare;

  bit_a = bit_object_a;
  bit_b = bit_object_b;
  if (*bit_a > *bit_b) {
    compare = 1;
  } else if (*bit_a < *bit_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *h_core_bit_copy(void *bit_object)
{
  assert(bit_object);
  h_core_bit_t *bit;
  h_core_bit_t *bit_copy;

  bit = bit_object;
  bit_copy = malloc(sizeof *bit_copy);
  if (bit_copy) {
    *bit_copy = *bit;
  } else {
    h_core_trace("mallloc");
  }

  return bit_copy;
}

void h_core_bit_destroy(void *bit_object)
{
  assert(bit_object);
  free(bit_object);
}
