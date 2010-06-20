#include "h/core/pair.h"
#include "h/core/pair_objectindex.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"

int h_core_pair_objectindex_compare(void *pair_object_a,
    void *pair_object_b)
{
  assert(pair_object_a);
  assert(pair_object_b);
  h_core_pair_t *pair_a;
  h_core_pair_t *pair_b;
  unsigned long *object_a;
  unsigned long *object_b;
  int compare_result;
  h_core_compare_f compare;
  h_core_objectey_t *left_objectey;

  pair_a = pair_object_a;
  pair_b = pair_object_b;

  object_a = h_core_pair_get_left(pair_a);
  object_b = h_core_pair_get_left(pair_b);

  left_objectey = h_core_pair_get_left_objectey(pair_a);
  compare = left_objectey->compare;
  compare_result = compare(object_a, object_b);

  return compare_result;
}

void *h_core_pair_objectindex_copy(void *pair_object)
{
  h_core_trace_exit("TODO: implement");
  return NULL;
}
