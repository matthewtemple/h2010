#include "h/container/array.h"
#include "h/container/set.h"
#include "h/external/external.h"

#if defined H_CONTAINER_SET_BINARY_TREE_IMPL
#include "h/container/set.binary_tree.impl.c"
#elif defined H_CONTAINER_SET_BTREE_IMPL
#include "h/container/set.btree.impl.c"
#elif defined H_CONTAINER_SET_HASH_MAP_IMPL
#include "h/container/set.hash_map.impl.c"
#elif defined H_CONTAINER_SET_SKIP_LIST_IMPL
#include "h/container/set.skip_list.impl.c"
#else
#include "h/container/set.binary_tree.impl.c"
#endif

set_object_t *_h_container_set_find_first(h_container_set_t *set);

set_object_t *_h_container_set_find_next(h_container_set_t *set,
    set_object_t *set_object);

void _h_container_set_remove_set_object(h_container_set_t *set,
    set_object_t *set_object);

h_core_bool_t h_container_set_absorb(h_container_set_t *set,
    h_container_set_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  void *object;
  h_core_bool_t success;

  success = h_core_bool_true;

  h_container_set_iterate_start(absorb_these);
  while ((object = h_container_set_iterate_next(absorb_these))) {
    if (!h_container_set_add(set, object)) {
      success = h_core_bool_false;
      h_core_trace("h_container_set_add");
    }
  }

  return success;
}

h_core_bool_t h_container_set_absorb_array(h_container_set_t *set,
    h_container_array_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  void *object;
  h_core_bool_t success;

  success = h_core_bool_true;

  h_container_array_iterate_start(absorb_these);
  while ((object = h_container_array_iterate_next(absorb_these))) {
    if (!h_container_set_add(set, object)) {
      success = h_core_bool_false;
      h_core_trace("h_container_set_add");
    }
  }

  return success;
}

h_core_bool_t h_container_set_absorb_copy(h_container_set_t *set,
    h_container_set_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  assert(set->destroy);
  void *object;
  void *object_copy;
  h_core_bool_t success;

  success = h_core_bool_true;

  h_container_set_iterate_start(absorb_these);
  while ((object = h_container_set_iterate_next(absorb_these))) {
    object_copy = set->copy(object);
    if (object_copy) {
      if (!h_container_set_add(set, object_copy)) {
        set->destroy(object_copy);
      }
    } else {
      success = h_core_bool_false;
      h_core_trace("copy");
    }
  }

  return success;
}

h_core_bool_t h_container_set_absorb_list_copy(h_container_set_t *set,
    h_container_list_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  assert(set->destroy);
  void *object;
  void *object_copy;
  h_core_bool_t success;

  success = h_core_bool_true;

  h_container_list_iterate_start(absorb_these);
  while ((object = h_container_list_iterate_next(absorb_these))) {
    object_copy = set->copy(object);
    if (object_copy) {
      if (!h_container_set_add(set, object_copy)) {
        set->destroy(object_copy);
      }
    } else {
      success = h_core_bool_false;
      h_core_trace("copy");
    }
  }

  return success;
}

h_core_bool_t h_container_set_add_to_message(h_container_set_t *set,
    h_core_message_t *message, h_core_message_add_to_message_f add_to_message)
{
  h_core_bool_t success;
  long set_size;
  void *object;

  set_size = h_container_set_get_size(set);
  success = h_core_message_add_long(message, &set_size);
  h_container_set_iterate_start(set);
  while ((object = h_container_set_iterate_next(set))) {
    if (!add_to_message(object, message)) {
      success = h_core_bool_false;
      break;
    }
  }

  return success;
}

void h_container_set_clear(h_container_set_t *set)
{
  assert(set);

  h_container_set_iterate_start(set);
  while (h_container_set_iterate_next(set)) {
    h_container_set_iterate_remove(set);
  }
}

int h_container_set_compare(void *set_object_a,
    void *set_object_b)
{
  assert(set_object_a);
  assert(set_object_b);
  h_container_set_t *set_a;
  h_container_set_t *set_b;
  int compare_result;
  void *object_a;
  void *object_b;
  unsigned long size_a;
  unsigned long size_b;
  h_core_compare_f compare;

  set_a = set_object_a;
  set_b = set_object_b;

  size_a = h_container_set_get_size(set_a);
  size_b = h_container_set_get_size(set_b);

  compare_result = h_core_long_compare(&size_a, &size_b);
  if (0 == compare_result) {
    compare = h_container_set_get_compare_function(set_a);
    h_container_set_iterate_start(set_a);
    h_container_set_iterate_start(set_b);
    while ((object_a = h_container_set_iterate_next(set_a))) {
      object_b = h_container_set_iterate_next(set_b);
      compare_result = compare(object_a, object_b);
      if (0 != compare_result) {
        break;
      }
    }
  }

  return compare_result;
}

h_core_bool_t h_container_set_contains(h_container_set_t *superset,
    h_container_set_t *subset)
{
  assert(superset);
  assert(subset);
  h_core_bool_t contains_it;
  void *object;

  contains_it = h_core_bool_true;

  h_container_set_iterate_start(subset);
  while ((object = h_container_set_iterate_next(subset))) {
    if (!h_container_set_find(superset, object)) {
      contains_it = h_core_bool_false;
      break;
    }
  }

  return contains_it;
}

void *h_container_set_copy(void *set_object)
{
  assert(set_object);
  h_container_set_t *set;
  h_container_set_t *copy;
  void *original_object;
  void *copied_object;

  set = set_object;

  copy = h_container_set_create(set->compare, set->copy, set->destroy);
  if (copy) {
    h_container_set_iterate_start(set);
    while ((original_object = h_container_set_iterate_next(set))) {
      copied_object = set->copy(original_object);
      if (copied_object) {
        if (!h_container_set_add(copy, copied_object)) {
          h_core_trace("h_container_set_add");
          set->destroy(copied_object);
          h_container_set_destroy(copy);
          copy = NULL;
          break;
        }
      } else {
        h_core_trace("copy");
        h_container_set_destroy(copy);
        copy = NULL;
        break;
      }
    }
  } else {
    h_core_trace("h_container_set_create");
  }

  return copy;
}

h_container_set_t *h_container_set_create_from_message
(h_core_compare_f compare, h_core_copy_f copy,
    h_core_destroy_f destroy, h_core_message_t *message,
    h_core_message_create_from_message_f create_from_message)
{
  h_container_set_t *set;
  long set_size;
  long each_object;
  void *object;

  set = h_container_set_create(compare, copy, destroy);
  if (set) {
    set_size = h_core_message_take_long_value(message);
    for (each_object = 0; each_object < set_size; each_object++) {
      object = create_from_message(message);
      if (!h_container_set_add(set, object)) {
        h_core_trace("h_container_set_add");
      }
    }
  } else {
    h_core_trace("h_container_set_create");
  }

  return set;
}

h_container_set_t *h_container_set_create_from_union(h_container_set_t *set_a,
    h_container_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  h_container_set_t *union_set;
  void *object;

  union_set = h_container_set_copy(set_a);
  if (union_set) {
    h_container_set_iterate_start(set_b);
    while ((object = h_container_set_iterate_next(set_b))) {
      h_container_set_add(union_set, object);
    }
  } else {
    h_core_trace("h_container_set_copy");
  }

  return union_set;
}

h_container_set_t *h_container_set_difference(h_container_set_t *set_a,
    h_container_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  h_container_set_t *difference;

  difference = h_container_set_copy(set_a);
  if (difference) {
    if (!h_container_set_expunge(difference, set_b)) {
      h_core_trace("h_container_set_expunge");
      h_container_set_destroy(difference);
      difference = NULL;
    }
  } else {
    h_core_trace("h_container_set_copy");
  }

  return difference;
}

void h_container_set_dont_destroys(h_container_set_t *set)
{
  set->destroy = NULL;
}

h_core_bool_t h_container_set_expunge(h_container_set_t *set,
    h_container_set_t *expunge_these)
{
  assert(set);
  assert(expunge_these);
  h_core_bool_t success;
  void *object;

  h_container_set_iterate_start(expunge_these);
  while ((object = h_container_set_iterate_next(expunge_these))) {
    h_container_set_remove(set, object);
  }

  /*
    TODO: return codes in h_core_set interface don't allow us to determine
    success or failure here
  */
  success = h_core_bool_true;

  return success;
}

void *h_container_set_find_copy(h_container_set_t *set, void *decoy_object)
{
  void *found_object;
  void *found_object_copy;

  found_object = h_container_set_find(set, decoy_object);
  if (found_object) {
    found_object_copy = set->copy(found_object);
    if (!found_object_copy) {
      h_core_trace("copy");
    }
  } else {
    found_object_copy = NULL;
  }

  return found_object_copy;
}

void *h_container_set_find_take(h_container_set_t *set,
    void *decoy_object)
{
  void *object;

  object = h_container_set_find(set, decoy_object);
  if (object) {
    if (!h_container_set_remove(set, decoy_object)) {
      h_core_trace("h_container_set_remove");
    }
  }

  return object;
}

h_container_array_t *h_container_set_get_as_array(h_container_set_t *set)
{
  assert(set);
  h_container_array_t *array;
  void *object;
  unsigned long object_count;
  unsigned long object_index;

  object_count = set->size;

  array = h_container_array_create(object_count, set->compare, set->copy,
      H_CORE_NO_DESTROY_FUNCTION);
  if (array) {
    object_index = 0;
    h_container_set_iterate_start(set);
    while ((object = h_container_set_iterate_next(set))) {
      h_container_array_add(array, object_index, object);
      object_index++;
    }
  } else {
    h_core_trace("h_container_array_create");
  }

  return array;
}

h_container_list_t *h_container_set_get_as_list(h_container_set_t *set)
{
  assert(set);
  h_container_list_t *list;
  void *object;

  list = h_container_list_create(set->compare, set->copy,
      H_CORE_NO_DESTROY_FUNCTION);
  if (list) {
    h_container_set_iterate_start(set);
    while ((object = h_container_set_iterate_next(set))) {
      h_container_list_add_last(list, object);
    }
  } else {
    h_core_trace("h_container_list_create");
  }

  return list;
}

char *h_container_set_get_as_delimited_string(h_container_set_t *set,
    h_core_get_as_string_f get_as_string, const char *delimiter)
{
  assert(set);
  assert(get_as_string);
  assert(delimiter);
  char *string;
  void *object;
  char *object_string;
  unsigned long object_index;
  unsigned long last_object_index;
  h_core_bool_t success;

  string = NULL;
  last_object_index = set->size - 1;
  success = h_core_bool_true;

  object_index = 0;
  h_container_set_iterate_start(set);
  while ((object = h_container_set_iterate_next(set))) {
    object_string = get_as_string(object);
    if (object_string) {
      string = h_core_string_append(string, object_string);
      free(object_string);
      if (string) {
        if (object_index != last_object_index) {
          string = h_core_string_append(string, delimiter);
          if (!string) {
            success = h_core_bool_false;
            h_core_trace("h_core_string_append");
            break;
          }
        }
      } else {
        success = h_core_bool_false;
        h_core_trace("h_core_string_append");
        break;
      }
    } else {
      success = h_core_bool_false;
      h_core_trace("get_as_string");
      break;
    }
    object_index++;
  }

  if (!success && string) {
    free(string);
    string = NULL;
  }

  return string;
}

h_core_compare_f h_container_set_get_compare_function(h_container_set_t *set)
{
  return set->compare;
}

h_core_copy_f h_container_set_get_copy_function(h_container_set_t *set)
{
  return set->copy;
}

h_core_destroy_f h_container_set_get_destroy_function(h_container_set_t *set)
{
  return set->destroy;
}

unsigned long h_container_set_get_size(h_container_set_t *set)
{
  return set->size;
}

void *h_container_set_iterate_next(h_container_set_t *set)
{
  assert(set);
  void *next_object;
  set_object_t *successor;

  if (set->iterator) {
    if (set->iterate_first) {
      next_object = set->iterator->object;
      set->iterate_first = h_core_bool_false;
    } else {
      if (set->iterate_remove) {
        successor = _h_container_set_find_next(set, set->iterator);
        _h_container_set_remove_set_object(set, set->iterator);
        set->iterator = successor;
        set->iterate_remove = h_core_bool_false;
      } else {
        set->iterator = _h_container_set_find_next(set, set->iterator);
      }
      if (set->iterator) {
        next_object = set->iterator->object;
      } else {
        next_object = NULL;
      }
    }
  } else {
    next_object = NULL;
  }

  return next_object;
}

void h_container_set_iterate_remove(h_container_set_t *set)
{
  set->iterate_remove = h_core_bool_true;
}

void h_container_set_iterate_start(h_container_set_t *set)
{
  assert(set);

  set->iterator = _h_container_set_find_first(set);
  set->iterate_remove = h_core_bool_false;
  set->iterate_first = h_core_bool_true;
}

void h_container_set_lock(h_container_set_t *set)
{
  pthread_mutex_lock(&set->mutex);
}

h_core_bool_t h_container_set_overlaps(h_container_set_t *set_a,
    h_container_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  h_core_bool_t overlaps;
  void *object;
  unsigned long set_a_size;
  unsigned long set_b_size;
  h_container_set_t *larger_set;
  h_container_set_t *smaller_set;

  set_a_size = h_container_set_get_size(set_a);
  set_b_size = h_container_set_get_size(set_b);
  if (set_a_size > set_b_size) {
    larger_set = set_a;
    smaller_set = set_b;
  } else {
    larger_set = set_b;
    smaller_set = set_a;
  }

  overlaps = h_core_bool_false;
  h_container_set_iterate_start(smaller_set);
  while ((object = h_container_set_iterate_next(smaller_set))) {
    if (h_container_set_find(larger_set, object)) {
      overlaps = h_core_bool_true;
      break;
    }
  }

  return overlaps;
}

void h_container_set_unlock(h_container_set_t *set)
{
  pthread_mutex_unlock(&set->mutex);
}
