#include "h/container/map.h"
#include "h/core/long.h"
#include "h/core/objects.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"

int main(int argc, char *argv[])
{
  h_container_map_t *map;
  h_core_objects_t objects;

  h_core_objects_init(&objects);

  char *a_left = "apple";
  unsigned long a_right = 0;

  char *b_left = "bear";
  unsigned long b_right = 1;

  unsigned long *x_right;

  map = h_container_map_create(&objects.string_objectey,
      &objects.long_objectey, H_CONTAINER_MAP_DONT_DESTROY);
  if (!map) {
    h_core_trace_exit("h_container_map_create");
  }

  if (!h_container_map_add(map, a_left, &a_right)) {
    h_core_trace_exit("h_container_map_add");
  }

  if (!h_container_map_add(map, b_left, &b_right)) {
    h_core_trace_exit("h_container_map_add");
  }

  h_container_map_print(map);

  x_right = h_container_map_find(map, b_left);
  if (x_right) {
    printf(":%lu:\n", *x_right);
  } else {
    h_core_trace_exit("h_container_map_find");
  }

  h_container_map_destroy(map);
  h_core_objects_free(&objects);

  return 0;
}
