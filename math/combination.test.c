#include "h/container/set.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/math/combination.h"

int main(int argc, char *argv[])
{
  h_math_combination_t *combination;
  h_container_set_t *set;
  h_container_set_t *subset;

  set = h_container_set_create(h_core_string_compare, h_core_string_copy,
      H_CORE_NO_DESTROY_FUNCTION);
  if (!set) {
    h_core_trace_exit("h_container_set_create");
  }

  h_container_set_add(set, "rooster");
  h_container_set_add(set, "cow");
  h_container_set_add(set, "pig");
  h_container_set_add(set, "duck");

  combination = h_math_combination_create(set);
  if (!combination) {
    h_core_trace_exit("h_math_combination_create");
  }

  h_math_combination_iterate_start(combination);
  while ((subset = h_math_combination_iterate_next(combination))) {
    h_container_set_print(subset, h_core_string_get_as_string);
    h_container_set_destroy(subset);
  }

  h_math_combination_destroy(combination);
  h_container_set_destroy(set);

  return 0;
}
