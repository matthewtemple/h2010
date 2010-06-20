#include "h/core/object.h"
#include "h/core/pair.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_core_pair_t {
  void *left;
  h_core_objectey_t *left_objectey;

  void *right;
  h_core_objectey_t *right_objectey;

  h_core_bool_t destroy;
};

int h_core_pair_compare(void *pair_object_a, void *pair_object_b)
{
  h_core_trace_exit("TODO: implement");
  return 0;
}

int h_core_pair_compare_left(void *pair_object_a, void *pair_object_b)
{
  assert(pair_object_a);
  assert(pair_object_b);
  h_core_pair_t *pair_a;
  h_core_pair_t *pair_b;
  int compare_result;
  h_core_compare_f compare;

  pair_a = pair_object_a;
  pair_b = pair_object_b;

  compare = pair_a->left_objectey->compare;
  compare_result = compare(pair_a->left, pair_b->left);

  return compare_result;
}

int h_core_pair_compare_right(void *pair_object_a, void *pair_object_b)
{
  assert(pair_object_a);
  assert(pair_object_b);
  h_core_pair_t *pair_a;
  h_core_pair_t *pair_b;
  int compare_result;
  h_core_compare_f compare;

  pair_a = pair_object_a;
  pair_b = pair_object_b;

  compare = pair_a->right_objectey->compare;
  compare_result = compare(pair_a->right, pair_b->right);

  return compare_result;
}

void *h_core_pair_copy(void *pair_object)
{
  h_core_trace_exit("TODO: implement");
  return NULL;
}

h_core_pair_t *h_core_pair_create(void *left, h_core_objectey_t *left_objectey,
    void *right, h_core_objectey_t *right_objectey, h_core_bool_t destroy)
{
  assert(left);
  assert(left_objectey);
  assert(right);
  assert(right_objectey);
  h_core_pair_t *pair;

  pair = malloc(sizeof *pair);
  if (pair) {
    pair->left = left;
    pair->left_objectey = left_objectey;
    pair->right = right;
    pair->right_objectey = right_objectey;
    pair->destroy = destroy;
  } else {
    h_core_trace("malloc");
  }

  return pair;
}

h_core_pair_t *h_core_pair_create_decoy(void *left,
    h_core_objectey_t *left_objectey)
{
  assert(left);
  assert(left_objectey);
  h_core_pair_t *pair;

  pair = malloc(sizeof *pair);
  if (pair) {
    pair->left = left;
    pair->left_objectey = left_objectey;
  } else {
    h_core_trace("malloc");
  }

  return pair;
}

void h_core_pair_destroy(void *pair_object)
{
  assert(pair_object);
  h_core_pair_t *pair;

  pair = pair_object;

  if (pair->destroy) {
    if (pair->left_objectey->destroy) {
      pair->left_objectey->destroy(pair->left);
    }
    if (pair->right_objectey->destroy) {
      pair->right_objectey->destroy(pair->right);
    }
  }

  free(pair);
}

void h_core_pair_destroy_decoy(h_core_pair_t *pair)
{
  free(pair);
}

char *h_core_pair_get_as_string(void *pair_object)
{
  assert(pair_object);
  h_core_pair_t *pair;
  char *string;
  h_core_get_as_string_f left_get_as_string;
  h_core_get_as_string_f right_get_as_string;
  char *left_string;
  char *right_string;

  pair = pair_object;

  left_get_as_string = pair->left_objectey->get_as_string;
  right_get_as_string = pair->right_objectey->get_as_string;

  left_string = left_get_as_string(pair->left);
  right_string = right_get_as_string(pair->right);

  string = h_core_string_append(NULL, right_string);
  string = h_core_string_append(string, ":");
  string = h_core_string_append(string, right_string);

  free(left_string);
  free(right_string);

  return string;
}

void *h_core_pair_get_left(h_core_pair_t *pair)
{
  return pair->left;
}

h_core_objectey_t *h_core_pair_get_left_objectey(h_core_pair_t *pair)
{
  return pair->left_objectey;
}

void *h_core_pair_get_right(h_core_pair_t *pair)
{
  return pair->right;
}

h_core_objectey_t *h_core_pair_get_right_objectey(h_core_pair_t *pair)
{
  return pair->right_objectey;
}

void h_core_pair_init_objectey(h_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->compare = h_core_pair_compare;
  objectey->copy = h_core_pair_copy;
  objectey->destroy = h_core_pair_destroy;
  objectey->get_as_string = h_core_pair_get_as_string;
}
