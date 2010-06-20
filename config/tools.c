#include "h/config/tools.h"
#include "h/config/options.h"
#include "h/core/tools.h"

void h_config_disable_if_running_batch_tests(int argc, char *argv[])
{
  if (h_config_running_batch_tests(argc, argv)) {
    printf("  %s disabled in batch test mode\n", *argv);
    exit(0);
  }
}

h_core_bool_t h_config_running_batch_tests(int argc, char *argv[])
{
  h_config_options_t *options;
  h_core_bool_t batch;
  h_core_objects_t objects;

  batch = h_core_bool_false;
  h_core_objects_init(&objects);

  options = h_config_options_create(argc, argv, &objects);
  if (options) {
    if (h_config_options_find(options, "running-batch-tests")) {
      batch = h_core_bool_true;
    }
    h_config_options_destroy(options);
  } else {
    h_core_trace("h_config_options_create");
  }

  h_core_objects_free(&objects);

  return batch;
}
