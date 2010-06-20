#include "h/container/set.h"
#include "h/core/long.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"

#define NO_LEFT_OBJECT NULL
#define NO_RIGHT_OBJECT NULL
#define NO_PARENT_OBJECT NULL

struct set_object_t;
typedef struct set_object_t set_object_t;

struct set_object_t {
  void *object;
  set_object_t *parent;
  set_object_t *left;
  set_object_t *right;
};

struct h_container_set_t {
  h_core_copy_f copy;
  h_core_compare_f compare;
  h_core_destroy_f destroy;
  h_core_get_as_string_f get_object_as_string;
  unsigned long size;
  h_core_bool_t iterate_remove;
  h_core_bool_t iterate_first;
  set_object_t *iterator;

  set_object_t *base;

  pthread_mutex_t mutex;
};

static void assign_to_child(set_object_t *parent, set_object_t *child,
    set_object_t *new_value);

static set_object_t *find_first_parent_greater_than_me(h_container_set_t *set,
    set_object_t *set_object);

static set_object_t *find_in_order_predecessor(h_container_set_t *set,
    set_object_t *set_object);

static set_object_t *find_in_order_successor(h_container_set_t *set,
    set_object_t *set_object);

static set_object_t *find_set_object_containing(h_container_set_t *set,
    set_object_t *base_set_object, void *object);

static void print(h_container_set_t *set, set_object_t *base_set_object);

static set_object_t *put_object(h_container_set_t *set,
    set_object_t *base_set_object, void *object,
    set_object_t *parent);

static void remove_set_object_both_children(h_container_set_t *set,
    set_object_t *set_object);
static void remove_set_object_left_child_only(h_container_set_t *set,
    set_object_t *set_object);
static void remove_set_object_no_children(h_container_set_t *set,
    set_object_t *set_object);
static void remove_set_object_right_child_only(h_container_set_t *set,
    set_object_t *set_object);

static set_object_t *set_object_create(void *object,
    set_object_t *parent, set_object_t *left, set_object_t *right);

static void set_object_destroy(h_container_set_t *set,
    set_object_t *set_object);

set_object_t *_h_container_set_find_first(h_container_set_t *set)
{
  set_object_t *first;

  first = set->base;
  if (first && first->left) {
    while (first->left) {
      first = first->left;
    }
  }

  return first;
}

set_object_t *_h_container_set_find_next(h_container_set_t *set,
    set_object_t *set_object)
{
  assert(set_object);
  set_object_t *successor;

  successor = set_object->right;
  if (successor) {
    while (successor->left) {
      successor = successor->left;
    }
  } else {
    if (set_object->parent) {
      if (set_object == set_object->parent->left) {
        successor = set_object->parent;
      } else {
        successor = find_first_parent_greater_than_me(set, set_object);
      }

    } else {
      successor = NULL;
    }
  }

  return successor;
}

void _h_container_set_remove_set_object(h_container_set_t *set,
    set_object_t *set_object)
{
  assert(set);
  assert(set_object);

  if (!set_object->left && !set_object->right) {
    remove_set_object_no_children(set, set_object);

  } else if (set_object->left && !set_object->right) {
    remove_set_object_left_child_only(set, set_object);

  } else if (!set_object->left && set_object->right) {
    remove_set_object_right_child_only(set, set_object);

  } else {
    remove_set_object_both_children(set, set_object);

  }

  set->size--;
}

void assign_to_child(set_object_t *parent, set_object_t *child,
    set_object_t *new_value)
{
  assert(parent);

  if (child == parent->left) {
    parent->left = new_value;
  } else if (child == parent->right) {
    parent->right = new_value;
  }
  if (new_value) {
    new_value->parent = parent;
  }
}

set_object_t *find_first_parent_greater_than_me(h_container_set_t *set,
    set_object_t *set_object)
{
  assert(set);
  assert(set_object);
  int compare;
  set_object_t *parent;

  parent = set_object->parent;
  if (parent) {
    compare = set->compare(parent->object,
        set_object->object);
    while (compare < 0) {
      parent = parent->parent;
      if (parent) {
        compare = set->compare(parent->object,
            set_object->object);
      } else {
        break;
      }
    }
  }

  return parent;
}

set_object_t *find_in_order_predecessor(h_container_set_t *set,
    set_object_t *set_object)
{
  assert(set_object);
  set_object_t *predecessor;

  predecessor = set_object->left;
  if (predecessor) {
    while (predecessor->right) {
      predecessor = predecessor->right;
    }
  } else {
    predecessor = NULL;
  }

  return predecessor;
}

set_object_t *find_in_order_successor(h_container_set_t *set,
    set_object_t *set_object)
{
  assert(set_object);
  set_object_t *successor;

  successor = set_object->right;
  if (successor) {
    while (successor->left) {
      successor = successor->left;
    }
  } else {
    successor = NULL;
  }

  return successor;
}

set_object_t *find_set_object_containing(h_container_set_t *set,
    set_object_t *base_set_object, void *object)
{
  int compare;
  set_object_t *containing_object;

  if (base_set_object) {
    compare = set->compare(object, base_set_object->object);
    if (compare < 0) {
      containing_object = find_set_object_containing(set,
          base_set_object->left, object);
    } else if (compare > 0) {
      containing_object = find_set_object_containing(set,
          base_set_object->right, object);
    } else {
      containing_object = base_set_object;
    }
  } else {
    containing_object = NULL;
  }

  return containing_object;
}

h_core_bool_t h_container_set_add(h_container_set_t *set,
    void *object)
{
  assert(set);
  assert(object);
  set_object_t *set_object;
  h_core_bool_t success;

  set_object = put_object(set, set->base, object, NO_PARENT_OBJECT);
  if (set_object) {
    success = h_core_bool_true;
    if (!set->base) {
      set->base = set_object;
    }
  } else {
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_container_set_add_replace(h_container_set_t *set,
    void *object)
{
  h_core_bool_t success;

  if (h_container_set_find(set, object)) {
    h_container_set_remove(set, object);
  }
  success = h_container_set_add(set, object);

  return success;
}

h_container_set_t *h_container_set_create
(h_core_compare_f compare, h_core_copy_f copy,
    h_core_destroy_f destroy)
{
  assert(compare);
  h_container_set_t *set;

  set = malloc(sizeof *set);
  if (set) {
    set->copy = copy;
    set->compare = compare;
    set->destroy = destroy;
    set->get_object_as_string = NULL;
    set->base = NULL;
    set->size = 0;
    if (0 != pthread_mutex_init(&set->mutex, NULL)) {
      h_core_trace("pthread_mutex_init");
    }
  }

  return set;
}

void h_container_set_destroy(void *set_object)
{
  assert(set_object);
  h_container_set_t *set;

  set = set_object;

  h_container_set_clear(set);
  if (0 != pthread_mutex_destroy(&set->mutex)) {
    h_core_trace("pthread_mutex_destroy");
  }
  free(set);
}

void *h_container_set_find(h_container_set_t *set,
    void *decoy_object)
{
  set_object_t *set_object;
  void *found_object;

  set_object = find_set_object_containing(set, set->base, decoy_object);
  if (set_object) {
    found_object = set_object->object;
  } else {
    found_object = NULL;
  }

  return found_object;
}

void *h_container_set_find_any(h_container_set_t *set)
{
  assert(set);
  void *any_object;

  if (set->base) {
    any_object = set->base->object;
  } else {
    any_object = NULL;
  }

  return any_object;
}

void h_container_set_print(h_container_set_t *set,
    h_core_get_as_string_f get_object_as_string)
{
  set->get_object_as_string = get_object_as_string;
  printf("set (%lu items) : ", set->size);
  print(set, set->base);
  printf("\n");
}

h_core_bool_t h_container_set_remove(h_container_set_t *set,
    void *object)
{
  h_core_bool_t success;
  set_object_t *set_object;

  set_object = find_set_object_containing(set, set->base, object);
  if (set_object) {
    _h_container_set_remove_set_object(set, set_object);
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

void print(h_container_set_t *set, set_object_t *base_set_object)
{
  char *object_string;

  if (base_set_object) {
    object_string = set->get_object_as_string(base_set_object->object);
    if (object_string) {
      printf("%s", object_string);
      free(object_string);
    } else {
      h_core_trace("get_as_string");
    }
    printf("(");
    print(set, base_set_object->left);
    printf(",");
    print(set, base_set_object->right);
    printf(")");
  }
}

set_object_t *put_object(h_container_set_t *set, set_object_t *base_set_object,
    void *object, set_object_t *parent)
{
  int compare;
  set_object_t *new_set_object;

  if (!base_set_object) {
    new_set_object = set_object_create(object, parent, NO_LEFT_OBJECT,
        NO_RIGHT_OBJECT);
    if (new_set_object) {
      set->size++;
    } else {
      h_core_trace("set_object_create");
    }
  } else {
    compare = set->compare(object, base_set_object->object);
    if (compare < 0) {
      new_set_object = put_object(set, base_set_object->left, object,
          base_set_object);
      if (!base_set_object->left) {
        base_set_object->left = new_set_object;
      }
    } else if (compare > 0) {
      new_set_object = put_object(set, base_set_object->right, object,
          base_set_object);
      if (!base_set_object->right) {
        base_set_object->right = new_set_object;
      }
    } else {
      new_set_object = NULL;
    }
  }

  return new_set_object;
}

void remove_set_object_both_children(h_container_set_t *set,
    set_object_t *set_object)
{
  set_object_t *set_object_2;

  if (0 == (random() % 2)) {

    set_object_2 = find_in_order_predecessor(set, set_object);

    if (set_object_2->parent->right == set_object_2) {
      assign_to_child(set_object_2->parent, set_object_2->parent->right,
          set_object_2->left);
    }

    if (set_object->left != set_object_2) {
      set_object_2->left = set_object->left;
      if (set_object->left) {
        set_object->left->parent = set_object_2;
      }
    }

    set_object_2->right = set_object->right;
    if (set_object->right) {
      set_object->right->parent = set_object_2;
    }

    if (set_object->parent) {
      assign_to_child(set_object->parent, set_object, set_object_2);
    } else {
      set->base = set_object_2;
      set_object_2->parent = NULL;
    }

  } else {

    set_object_2 = find_in_order_successor(set, set_object);

    if (set_object_2->parent->left == set_object_2) {
      assign_to_child(set_object_2->parent, set_object_2->parent->left,
          set_object_2->right);
    }

    if (set_object->right != set_object_2) {
      set_object_2->right = set_object->right;
      if (set_object->right) {
        set_object->right->parent = set_object_2;
      }
    }

    set_object_2->left = set_object->left;
    if (set_object->left) {
      set_object->left->parent = set_object_2;
    }

    if (set_object->parent) {
      assign_to_child(set_object->parent, set_object, set_object_2);
    } else {
      set->base = set_object_2;
      set_object_2->parent = NULL;
    }

  }

  set_object_destroy(set, set_object);
}

void remove_set_object_left_child_only(h_container_set_t *set,
    set_object_t *set_object)
{
  if (set_object->parent) {
    assign_to_child(set_object->parent, set_object, set_object->left);
  } else {
    set->base = set_object->left;
    set_object->left->parent = NULL;
  }
  set_object_destroy(set, set_object);
}

void remove_set_object_no_children(h_container_set_t *set,
    set_object_t *set_object)
{
  if (set_object->parent) {
    assign_to_child(set_object->parent, set_object, NULL);
  } else {
    set->base = NULL;
  }
  set_object_destroy(set, set_object);
}

void remove_set_object_right_child_only(h_container_set_t *set,
    set_object_t *set_object)
{
  if (set_object->parent) {
    assign_to_child(set_object->parent, set_object, set_object->right);
  } else {
    set->base = set_object->right;
    set_object->right->parent = NULL;
  }
  set_object_destroy(set, set_object);
}

set_object_t *set_object_create(void *object, set_object_t *parent,
    set_object_t *left, set_object_t *right)
{
  set_object_t *set_object;

  set_object = malloc(sizeof *set_object);
  if (set_object) {
    set_object->object = object;
    set_object->parent = parent;
    set_object->left = left;
    set_object->right = right;
  } else {
    h_core_trace("malloc");
  }

  return set_object;
}

void set_object_destroy(h_container_set_t *set, set_object_t *set_object)
{
  assert(set);
  assert(set_object);

  if (set_object->object && set->destroy) {
    set->destroy(set_object->object);
  }
  free(set_object);
}
