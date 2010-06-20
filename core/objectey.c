#include "h/core/objectey.h"
#include "h/external/external.h"

void h_core_objectey_init(h_core_objectey_t *objectey,
    h_core_compare_f compare, h_core_copy_f copy,
    h_core_destroy_f destroy,
    h_core_get_as_string_f get_as_string)
{
  assert(objectey);

  objectey->compare = compare;
  objectey->copy = copy;
  objectey->destroy = destroy;
  objectey->get_as_string = get_as_string;
}
