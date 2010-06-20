#include "h/core/tools.h"
#include "h/config/environment.h"

struct h_config_environment_t {
};

h_config_environment_t *h_config_environment_create()
{
  h_config_environment_t *environment;

  environment = malloc(sizeof *environment);
  if (environment) {

  } else {
    h_core_trace("malloc");
  }

  return environment;
}

void h_config_environment_destroy(h_config_environment_t *environment)
{
  assert(environment);
  free(environment);
}

h_core_bool_t h_config_environment_find(h_config_environment_t *environment,
    char *name)
{
  assert(environment);
  assert(name);

  return h_core_bool_false;
}

h_core_bool_t h_config_environment_find_as_double
(h_config_environment_t *environment, char *name, double *value,
    double default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return h_core_bool_false;
}

h_core_bool_t h_config_environment_find_as_string
(h_config_environment_t *environment, char *name, char **value,
    char *default_value)
{
  assert(environment);
  assert(name);
  assert(value);
  assert(default_value);

  return h_core_bool_false;
}

h_core_bool_t h_config_environment_find_as_unsigned_long
(h_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return h_core_bool_false;
}

h_core_bool_t h_config_environment_find_as_unsigned_short
(h_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return h_core_bool_false;
}

h_core_bool_t h_config_environment_find_list_as_strings
(h_config_environment_t *environment, char *name, h_container_list_t **list)
{
  assert(environment);
  assert(name);
  assert(list);

  return h_core_bool_false;
}
