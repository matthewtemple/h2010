#include "h/core/object.h"
#include "h/core/tools.h"
#include "h/external/external.h"

void h_core_print(void *object, h_core_get_as_string_f get_object_as_string)
{
  assert(object);
  assert(get_object_as_string);
  char *object_string;

  object_string = get_object_as_string(object);
  if (object_string) {
    printf("%s\n", object_string);
    free(object_string);
  } else {
    h_core_trace("get_as_string");
  }
}
