#include "h/container/set.h"
#include "h/core/long.h"
#include "h/core/tools.h"
#include "h/core/uuid.h"
#include "h/external/external.h"

#define ITERATIONS 10000
#define TEST_USING_LONGS h_core_bool_false

static void print_uuid(h_core_uuid_t *uuid, const char *name);

static void print_uuid(h_core_uuid_t *uuid, const char *name)
{
  assert(uuid);
  assert(name);
  char *uuid_string;

  uuid_string = h_core_uuid_get_string(uuid);
  printf("%s=:%s:\n", name, uuid_string);
}

int main(int argc, char *argv[])
{
  h_container_set_t *set;
  long *l;
  unsigned long n;
  unsigned long items_added;
  h_core_uuid_t *uuid;

  printf("creating set...\n");
  if (TEST_USING_LONGS) {
    set = h_container_set_create(h_core_long_compare, h_core_long_copy,
        h_core_long_destroy);
  } else {
    set = h_container_set_create(h_core_uuid_compare, h_core_uuid_copy,
        h_core_uuid_destroy);
  }
  assert(set);

  printf("adding to set...\n");
  items_added = 0;
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      if (h_container_set_add(set, l)) {
        items_added++;
      } else {
        h_core_long_destroy(l);
      }
    } else {
      uuid = h_core_uuid_create();
      if (h_core_bool_false) {
        print_uuid(uuid, "uuid");
      }
      if (h_container_set_add(set, uuid)) {
        items_added++;
      } else {
        h_core_uuid_destroy(uuid);
      }
    }
  }
  printf("items added: %lu\n", items_added);
  printf("set size: %lu\n", h_container_set_get_size(set));

  printf("removing from set...\n");
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      h_container_set_remove(set, l);
      free(l);
    }
  }
  printf("set size: %lu\n", h_container_set_get_size(set));

  if (h_core_bool_true) {
    printf("doing iterate_remove on the rest of the set\n");
    h_container_set_iterate_start(set);
    if (TEST_USING_LONGS) {
      while ((l = h_container_set_iterate_next(set))) {
        h_container_set_iterate_remove(set);
      }
    }
    while ((uuid = h_container_set_iterate_next(set))) {
      h_container_set_iterate_remove(set);
    }
    printf("set size: %lu\n", h_container_set_get_size(set));
    assert(0 == h_container_set_get_size(set));
  }

  printf("clearing set...\n");
  h_container_set_clear(set);
  printf("set size: %lu\n", h_container_set_get_size(set));
  assert(0 == h_container_set_get_size(set));

  printf("destroying set...\n");
  h_container_set_destroy(set);

  return 0;
}
