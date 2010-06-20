#include "h/config/environment.h"
#include "h/config/file.h"
#include "h/config/options.h"
#include "h/config/system.h"
#include "h/core/tools.h"

struct h_config_system_t {
  h_config_file_t *file;
  h_config_environment_t *environment;
  h_config_options_t *options;
};

h_config_system_t *h_config_system_create(int argc, char *argv[],
    char *filename, h_core_objects_t *objects)
{
  assert(argv);
  assert(objects);
  h_config_system_t *system;
  h_core_bool_t so_far_so_good;

  system = malloc(sizeof *system);
  if (system) {
    system->environment = NULL;
    system->options = NULL;
    if (filename) {
      system->file = h_config_file_create(filename);
      if (system->file) {
        so_far_so_good = h_core_bool_true;
      } else {
        so_far_so_good = h_core_bool_false;
        h_core_trace("h_config_file_create");
      }
    } else {
      so_far_so_good = h_core_bool_true;
      system->file = NULL;
    }
  } else {
    so_far_so_good = h_core_bool_false;
    h_core_trace("malloc");
  }

  if (so_far_so_good) {
    system->environment = h_config_environment_create();
    if (!system->environment) {
      so_far_so_good = h_core_bool_false;
      h_core_trace("h_config_environment_create");
    }
  }

  if (so_far_so_good) {
    system->options = h_config_options_create(argc, argv, objects);
    if (!system->options) {
      so_far_so_good = h_core_bool_false;
      h_core_trace("h_config_options_create");
    }
  }

  if (!so_far_so_good && system) {
    if (system->file) {
      h_config_file_destroy(system->file);
    }
    if (system->environment) {
      h_config_environment_destroy(system->environment);
    }
    if (system->options) {
      h_config_options_destroy(system->options);
    }
    free(system);
    system = NULL;
  }

  return system;
}

void h_config_system_destroy(h_config_system_t *system)
{
  assert(system);

  if (system->file) {
    h_config_file_destroy(system->file);
  }
  h_config_environment_destroy(system->environment);
  h_config_options_destroy(system->options);
  free(system);
}

h_core_bool_t h_config_system_find(h_config_system_t *system, char *name)
{
  assert(system);
  assert(name);
  h_core_bool_t found_it;

  found_it = h_core_bool_false;

  if (h_config_options_find(system->options, name)) {
    found_it = h_core_bool_true;
  } else {
    if (h_config_environment_find(system->environment, name)) {
      found_it = h_core_bool_true;
    } else {
      if (system->file && h_config_file_find(system->file, name)) {
        found_it = h_core_bool_true;
      }
    }
  }

  return found_it;
}

h_core_bool_t h_config_system_find_as_double(h_config_system_t *system,
    char *name, double *value, double default_value)
{
  assert(system);
  assert(name);
  assert(value);
  h_core_bool_t found_it;

  found_it = h_core_bool_false;

  if (h_config_options_find_as_double
      (system->options, name, value, default_value)) {
    found_it = h_core_bool_true;
  } else {
    if (h_config_environment_find_as_double
        (system->environment, name, value, default_value)) {
      found_it = h_core_bool_true;
    } else {
      if (system->file && h_config_file_find_as_double
          (system->file, name, value, default_value)) {
        found_it = h_core_bool_true;
      }
    }
  }

  return found_it;
}

h_core_bool_t h_config_system_find_as_string(h_config_system_t *system,
    char *name, char **value, char *default_value)
{
  assert(system);
  assert(name);
  assert(value);
  assert(default_value);
  h_core_bool_t found_it;

  found_it = h_core_bool_false;

  if (h_config_options_find_as_string
      (system->options, name, value, default_value)) {
    found_it = h_core_bool_true;
  } else {
    if (h_config_environment_find_as_string
        (system->environment, name, value, default_value)) {
      found_it = h_core_bool_true;
    } else {
      if (system->file && h_config_file_find_as_string
          (system->file, name, value, default_value)) {
        found_it = h_core_bool_true;
      }
    }
  }

  return found_it;
}

h_core_bool_t h_config_system_find_as_unsigned_long(h_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(system);
  assert(name);
  assert(value);
  h_core_bool_t found_it;

  found_it = h_core_bool_false;

  if (h_config_options_find_as_unsigned_long
      (system->options, name, value, default_value)) {
    found_it = h_core_bool_true;
  } else {
    if (h_config_environment_find_as_unsigned_long
        (system->environment, name, value, default_value)) {
      found_it = h_core_bool_true;
    } else {
      if (system->file && h_config_file_find_as_unsigned_long
          (system->file, name, value, default_value)) {
        found_it = h_core_bool_true;
      }
    }
  }

  return found_it;
}

h_core_bool_t h_config_system_find_as_unsigned_short(h_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(system);
  assert(name);
  assert(value);
  h_core_bool_t found_it;

  found_it = h_core_bool_false;

  if (h_config_options_find_as_unsigned_short
      (system->options, name, value, default_value)) {
    found_it = h_core_bool_true;
  } else {
    if (h_config_environment_find_as_unsigned_short
        (system->environment, name, value, default_value)) {
      found_it = h_core_bool_true;
    } else {
      if (system->file && h_config_file_find_as_unsigned_short
          (system->file, name, value, default_value)) {
        found_it = h_core_bool_true;
      }
    }
  }

  return found_it;
}

h_core_bool_t h_config_system_find_list_as_strings(h_config_system_t *system,
    char *name, h_container_list_t **list)
{
  assert(system);
  assert(name);
  h_core_bool_t found_it;

  found_it = h_core_bool_false;

  if (h_config_options_find_list_as_strings(system->options, name, list)) {
    found_it = h_core_bool_true;
  } else {
    if (h_config_environment_find_list_as_strings
        (system->environment, name, list)) {
      found_it = h_core_bool_true;
    } else {
      if (system->file
          && h_config_file_find_list_as_strings(system->file, name, list)) {
        found_it = h_core_bool_true;
      }
    }
  }

  return found_it;
}
