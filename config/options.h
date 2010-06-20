#ifndef h_config_options_h
#define h_config_options_h

#include "h/container/list.h"
#include "h/core/objects.h"

struct h_config_options_t;
typedef struct h_config_options_t h_config_options_t;

h_config_options_t *h_config_options_create(int argc, char *argv[],
    h_core_objects_t *objects);

void h_config_options_destroy(h_config_options_t *options);

h_core_bool_t h_config_options_find(h_config_options_t *options, char *name);

h_core_bool_t h_config_options_find_as_double(h_config_options_t *options,
    char *name, double *value, double default_value);

h_core_bool_t h_config_options_find_as_string(h_config_options_t *options,
    char *name, char **value, char *default_value);

h_core_bool_t h_config_options_find_as_unsigned_short
(h_config_options_t *options, char *name, unsigned short *value,
    unsigned short default_value);

h_core_bool_t h_config_options_find_as_unsigned_long
(h_config_options_t *options, char *name, unsigned long *value,
    unsigned long default_value);

h_core_bool_t h_config_options_find_list_as_strings
(h_config_options_t *options, char *name, h_container_list_t **list);

#endif
