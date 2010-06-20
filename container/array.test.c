#include "h/container/array.h"
#include "h/core/string.h"
#include "h/external/external.h"

int main(int argc, char *argv[])
{
  h_container_array_t *array;
  void *object;
  char *object_string;
  unsigned long index;
  unsigned long iteration;

  array = h_container_array_create(4, h_core_string_compare,
      h_core_string_copy, h_core_string_destroy);

  h_container_array_add(array, 0, strdup("puppy"));
  h_container_array_add(array, 1, strdup("rooster"));
  h_container_array_add(array, 2, strdup("cow"));
  h_container_array_add(array, 3, strdup("spots"));

  for (index = 0; index < 4; index++) {
    object = h_container_array_find(array, index);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  for (iteration = 0; iteration < 16; iteration++) {
    object = h_container_array_find_random(array);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  h_container_array_iterate_start(array);
  while ((object = h_container_array_iterate_next(array))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  printf("\n");

  h_container_array_iterate_start(array);
  while ((object = h_container_array_iterate_next(array))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  h_container_array_destroy(array);

  h_container_array_t *array2;
  array2 = h_container_array_create(1, h_core_string_compare,
      h_core_string_copy, h_core_string_destroy);
  h_container_array_add(array2, 0, malloc(7));
  h_container_array_remove(array2, 0);
  h_container_array_destroy(array2);

  return 0;
}
