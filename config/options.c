#include "h/config/options.h"
#include "h/container/dumpster.h"
#include "h/container/map.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_config_options_t {
  h_container_map_t *options;
  h_container_dumpster_t *dumpster;
};

static void parse_options(h_config_options_t *options, int argc, char *argv[]);

h_config_options_t *h_config_options_create(int argc, char *argv[],
    h_core_objects_t *objects)
{
  h_config_options_t *options;
  h_core_bool_t so_far_so_good;

  options = malloc(sizeof *options);
  if (options) {
    options->dumpster = NULL;
    options->options = h_container_map_create(&objects->string_objectey,
        &objects->string_objectey, H_CONTAINER_MAP_DONT_DESTROY);
    if (options->options) {
      so_far_so_good = h_core_bool_true;
    } else {
      h_core_trace("h_container_map_create");
      so_far_so_good = h_core_bool_false;
    }
  } else {
    h_core_trace("malloc");
    so_far_so_good = h_core_bool_false;
  }

  if (so_far_so_good) {
    options->dumpster = h_container_dumpster_create(&objects->string_objectey);
    if (!options->dumpster) {
      h_core_trace("h_container_dumpster_create");
    }
  }

  if (so_far_so_good) {
    parse_options(options, argc, argv);
  }

  if (!so_far_so_good && options) {
    h_container_map_destroy(options->options);
    if (options->dumpster) {
      h_container_dumpster_destroy(options->dumpster);
    }
    free(options);
    options = NULL;
  }

  return options;
}

void h_config_options_destroy(h_config_options_t *options)
{
  assert(options);

  h_container_map_destroy(options->options);
  h_container_dumpster_destroy(options->dumpster);
  free(options);
}

h_core_bool_t h_config_options_find(h_config_options_t *options, char *name)
{
  assert(options);
  assert(name);
  h_core_bool_t found;

  if (h_container_map_find(options->options, name)) {
    found = h_core_bool_true;
  } else {
    found = h_core_bool_false;
  }

  return found;
}

h_core_bool_t h_config_options_find_as_double(h_config_options_t *options,
    char *name, double *value, double default_value)
{
  assert(options);
  assert(name);
  assert(value);
  h_core_bool_t found;
  char *value_string;

  value_string = h_container_map_find(options->options, name);
  if (value_string) {
    found = h_core_bool_true;
    *value = atof(value_string);
  } else {
    found = h_core_bool_false;
    *value = default_value;
  }

  return found;
}

h_core_bool_t h_config_options_find_as_string(h_config_options_t *options,
    char *name, char **value, char *default_value)
{
  assert(options);
  assert(name);
  assert(value);
  assert(default_value);
  h_core_bool_t found;

  *value = h_container_map_find(options->options, name);
  if (*value) {
    found = h_core_bool_true;
  } else {
    found = h_core_bool_false;
    *value = default_value;
  }

  return found;
}

h_core_bool_t h_config_options_find_as_unsigned_short
(h_config_options_t *options, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(options);
  assert(name);
  assert(value);
  h_core_bool_t found;
  char *value_string;

  value_string = h_container_map_find(options->options, name);
  if (value_string) {
    found = h_core_bool_true;
    *value = atoi(value_string);
  } else {
    found = h_core_bool_false;
    *value = default_value;
  }

  return found;
}

h_core_bool_t h_config_options_find_as_unsigned_long
(h_config_options_t *options, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(options);
  assert(name);
  assert(value);
  h_core_bool_t found;
  char *value_string;

  value_string = h_container_map_find(options->options, name);
  if (value_string) {
    found = h_core_bool_true;
    *value = atol(value_string);
  } else {
    found = h_core_bool_false;
    *value = default_value;
  }

  return found;
}

h_core_bool_t h_config_options_find_list_as_strings
(h_config_options_t *options, char *name, h_container_list_t **list)
{
  h_core_trace_exit("TODO: implement");
  return h_core_bool_false;
}

void parse_options(h_config_options_t *options, int argc, char *argv[])
{
  assert(options);
  int each_word;
  char *substring;
  char *name;
  char *value;

  for (each_word = 1; (each_word + 1) < argc; each_word += 2) {
    name = *(argv + each_word);
    substring = h_core_string_substring(name, 0, 2);
    if (substring) {
      if (0 == strcmp("--", substring)) {
        name = strdup(name + 2);
        if (name) {
          if (!h_container_dumpster_add(options->dumpster, name)) {
            h_core_trace("h_container_dumpster_add");
          }
          value = strdup(*(argv + each_word + 1));
          if (!h_container_dumpster_add(options->dumpster, value)) {
            h_core_trace("h_container_dumpster_add");
          }
          if (!h_container_map_add(options->options, name, value)) {
            h_core_trace("h_container_map_add");
            free(name);
          }
        } else {
          h_core_trace("strdup");
        }
      }
      free(substring);
    } else {
      h_core_trace("h_core_string_substring");
    }
  }
}
