#include "h/core/directory.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_core_directory_t {};

h_core_directory_t *h_core_directory_create()
{
  return NULL;
}

void h_core_directory_destroy(h_core_directory_t *h_core_directory)
{
}

h_core_bool_t h_core_directory_exists(char *directory_name)
{
  assert(directory_name);
  h_core_bool_t exists;
  DIR *dir;

  dir = opendir(directory_name);
  if (dir) {
    exists = h_core_bool_true;
    if (0 != closedir(dir)) {
      h_core_trace("closedir");
    }
  } else {
    exists = h_core_bool_false;
  }

  return exists;
}

h_core_bool_t h_core_directory_make(char *directory_name, mode_t mode)
{
  assert(directory_name);
  h_core_bool_t success;

  if (0 == mkdir(directory_name, mode)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}
