#ifndef h_config_environment_h
#define h_config_environment_h

#include "h/container/list.h"
#include "h/external/external.h"

struct h_config_environment_t;
typedef struct h_config_environment_t h_config_environment_t;

h_config_environment_t *h_config_environment_create();

void h_config_environment_destroy(h_config_environment_t *environment);

h_core_bool_t h_config_environment_find(h_config_environment_t *environment,
    char *name);

h_core_bool_t h_config_environment_find_as_double
(h_config_environment_t *environment, char *name, double *value,
    double default_value);

h_core_bool_t h_config_environment_find_as_string
(h_config_environment_t *environment, char *name, char **value,
    char *default_value);

h_core_bool_t h_config_environment_find_as_unsigned_long
(h_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value);

h_core_bool_t h_config_environment_find_as_unsigned_short
(h_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value);

h_core_bool_t h_config_environment_find_list_as_strings
(h_config_environment_t *environment, char *name, h_container_list_t **list);

#endif
