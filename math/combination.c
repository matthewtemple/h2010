#include "h/container/array.h"
#include "h/core/bitarray.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/math/combination.h"

struct h_math_combination_t {
  h_container_set_t *elements_set;
  h_container_array_t *elements;
  unsigned long element_count;
  h_core_bitarray_t *flags;

  h_core_compare_f compare;
  h_core_copy_f copy;
  h_core_destroy_f destroy;

  h_core_bool_t iterate_first;
};

static h_container_set_t *create_set_based_on_flags
(h_math_combination_t *combination);

h_container_set_t *create_set_based_on_flags(h_math_combination_t *combination)
{
  assert(combination);
  h_container_set_t *set;
  unsigned long each_element;
  void *object;
  void *object_copy;

  set = h_container_set_create(combination->compare,
      combination->copy, combination->destroy);
  if (set) {
    for (each_element = 0; each_element < combination->element_count;
         each_element++) {
      if (1 == h_core_bitarray_get_bit(combination->flags, each_element)) {
        object = h_container_array_find(combination->elements, each_element);
        object_copy = combination->copy(object);
        if (object_copy) {
          if (!h_container_set_add(set, object_copy)) {
            h_core_trace("h_container_set_add");
          }
        } else {
          h_core_trace("combination->copy");
        }
      }
    }
  } else {
    h_core_trace("h_container_set_create");
  }

  return set;
}

h_math_combination_t *h_math_combination_create(h_container_set_t *set)
{
  assert(set);
  assert(h_container_set_get_size(set) > 0);
  h_math_combination_t *combination;
  h_core_bool_t so_far_so_good;

  combination = malloc(sizeof *combination);
  if (combination) {
    combination->flags = NULL;
    combination->iterate_first = h_core_bool_false;
    combination->element_count = h_container_set_get_size(set);
    combination->compare = h_container_set_get_compare_function(set);
    combination->copy = h_container_set_get_copy_function(set);
    combination->destroy = h_container_set_get_destroy_function(set);

    combination->elements_set = h_container_set_copy(set);
    if (combination->elements_set) {
      so_far_so_good = h_core_bool_true;
    } else {
      so_far_so_good = h_core_bool_false;
      h_core_trace("h_container_set_copy");
    }

  } else {
    so_far_so_good = h_core_bool_false;

    h_core_trace("malloc");
  }

  if (so_far_so_good) {
    combination->elements = h_container_set_get_as_array
      (combination->elements_set);
    if (!combination->elements) {
      so_far_so_good = h_core_bool_false;
      h_core_trace("h_container_array_create_reference_from_set");
    }
  }

  if (so_far_so_good) {
    combination->flags = h_core_bitarray_create
      (combination->element_count);
    if (!combination->flags) {
      so_far_so_good = h_core_bool_false;
    }
  }

  if (!so_far_so_good && combination) {
    if (combination->elements_set) {
      h_container_set_destroy(combination->elements_set);
    }
    if (combination->elements) {
      h_container_array_destroy(combination->elements);
    }
    if (combination->flags) {
      h_core_bitarray_destroy(combination->flags);
    }
  }

  return combination;
}

void h_math_combination_destroy(h_math_combination_t *combination)
{
  assert(combination);

  h_container_set_destroy(combination->elements_set);
  h_container_array_destroy(combination->elements);
  h_core_bitarray_destroy(combination->flags);
  free(combination);
}

h_container_set_t *h_math_combination_iterate_next
(h_math_combination_t *combination)
{
  assert(combination);
  h_container_set_t *set;

  if (combination->iterate_first) {
    set = create_set_based_on_flags(combination);
    if (!set) {
      h_core_trace("create_set_based_on_flags");
    }
    combination->iterate_first = h_core_bool_false;
  } else {
    if (h_core_bitarray_increment(combination->flags)) {
      set = create_set_based_on_flags(combination);
      if (!set) {
        h_core_trace("create_set_based_on_flags");
      }
    } else {
      set = NULL;
    }
  }

  return set;
}

void h_math_combination_iterate_start(h_math_combination_t *combination)
{
  assert(combination);

  h_core_bitarray_unset_all(combination->flags);
  h_core_bitarray_set_bit(combination->flags, 0, 1);
  combination->iterate_first = h_core_bool_true;
}
