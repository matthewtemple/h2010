#include "h/container/list.h"
#include "h/core/string.h"
#include "h/external/external.h"

int main(int argc, char *argv[])
{
  h_container_list_t *list;
  void *object;
  char *object_string;
  unsigned long index;
  unsigned long iteration;
  h_container_list_t *strings;
  char *string;
  char *string_to_split;

  list = h_container_list_create(h_core_string_compare, h_core_string_copy,
      h_core_string_destroy);

  h_container_list_add_first(list, strdup("puppy"));
  h_container_list_add_first(list, strdup("rooster"));
  h_container_list_add_first(list, strdup("cow"));
  h_container_list_add_last(list, strdup("spots"));

  for (index = 0; index < 5; index++) {
    object = h_container_list_find_at(list, index);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  for (iteration = 0; iteration < 16; iteration++) {
    object = h_container_list_find_random(list);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  h_container_list_iterate_start(list);
  while ((object = h_container_list_iterate_next(list))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  printf("\n");

  h_container_list_iterate_start(list);
  while ((object = h_container_list_iterate_next(list))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  h_container_list_destroy(list);

  h_container_list_t *list2;
  list2 = h_container_list_create(h_core_string_compare, h_core_string_copy,
      h_core_string_destroy);
  h_container_list_add_first(list2, malloc(7));
  h_container_list_remove_first(list2);
  h_container_list_destroy(list2);

  string_to_split = malloc(sizeof("here I am"));
  strcpy(string_to_split, "here I am");

  strings = h_container_list_create_strings_from_string(string_to_split, " ");
  h_container_list_iterate_start(strings);
  while ((string = h_container_list_iterate_next(strings))) {
    printf(":%s:\n", string);
  }
  h_container_list_destroy(strings);

  free(string_to_split);

  return 0;
}
