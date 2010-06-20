#include "h/config/options.h"
#include "h/core/tools.h"
#include "h/external/external.h"

#define DEFAULT_MODE "modex"
#define DEFAULT_OPTION "optionx"

int main(int argc, char *argv[])
{
  h_core_objects_t objects;
  h_config_options_t *options;
  char *value;

  h_core_objects_init(&objects);

  options = h_config_options_create(argc, argv, &objects);
  if (!options) {
    h_core_trace_exit("h_config_options_create");
  }

  if (h_config_options_find_as_string(options, "mode", &value, DEFAULT_MODE)) {
    printf(":%s:\n", value);
  } else {
  }

  if (h_config_options_find_as_string
      (options, "option", &value, DEFAULT_OPTION)) {
    printf(":%s:\n", value);
  } else {
  }

  h_config_options_destroy(options);
  h_core_objects_free(&objects);

  return 0;
}
