#include "h/core/buffer.h"
#include "h/core/tools.h"
#include "h/external/external.h"

static char *initial_buffer = "happy";
static char *additional_buffer_0 = " day for fun on the beach";
static char *additional_buffer_1 = " with a white paper rabbyt";

int main(int argc, char *argv[])
{
  h_core_buffer_t *buffer;
  char *string;
  unsigned long string_size;

  buffer = h_core_buffer_create(initial_buffer, strlen(initial_buffer));
  if (!buffer) {
    h_core_trace_exit("h_core_buffer_create");
  }

  if (!h_core_buffer_append_string(buffer, additional_buffer_0,
          strlen(additional_buffer_0))) {
    h_core_trace_exit("h_core_buffer_append_string");
  }
  if (!h_core_buffer_append_string(buffer, additional_buffer_1,
          strlen(additional_buffer_1))) {
    h_core_trace_exit("h_core_buffer_append_string");
  }

  string = h_core_buffer_get_string(buffer, &string_size);
  printf("string_size:%lu\n", string_size);
  printf(":%s:\n", string);

  h_core_buffer_destroy(buffer);

  return 0;
}
