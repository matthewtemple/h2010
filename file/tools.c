#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/file/basic.h"
#include "h/file/tools.h"

h_core_bool_t h_file_count_lines_in_file(char *filename,
    unsigned long *line_count)
{
  assert(filename);
  assert(line_count);
  h_core_bool_t success;
  h_file_basic_t *file;

  file = h_file_basic_create(filename, H_FILE_MODE_OPEN_FOR_READ);
  if (file) {
    success = h_file_basic_get_line_count(file, line_count);
    h_file_basic_destroy(file);
  } else {
    success = h_core_bool_false;
    h_core_trace("h_file_basic_create");
  }

  return success;
}
