#include "h/core/tools.h"
#include "h/core/unsigned_short.h"
#include "h/external/external.h"

int h_core_unsigned_short_compare(void *unsigned_short_object_a,
    void *unsigned_short_object_b)
{
  unsigned short *short_a;
  unsigned short *short_b;
  int compare;

  short_a = unsigned_short_object_a;
  short_b = unsigned_short_object_b;

  if (*short_a > *short_b) {
    compare = 1;
  } else if (*short_a < *short_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *h_core_unsigned_short_copy
(void *unsigned_short_object)
{
  assert(unsigned_short_object);
  unsigned short *short_value;
  unsigned short *short_copy;

  short_value = unsigned_short_object;
  short_copy = malloc(sizeof *short_copy);
  if (short_copy) {
    *short_copy = *short_value;
  } else {
    h_core_trace("malloc");
  }

  return short_copy;
}

void h_core_unsigned_short_destroy(void *unsigned_short_object)
{
  assert(unsigned_short_object);
  free(unsigned_short_object);
}

void h_core_unsigned_short_print(void *unsigned_short_object)
{
  unsigned short *short_value;

  short_value = unsigned_short_object;
  printf("%d", *short_value);
}
