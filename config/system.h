#ifndef h_config_system_h
#define h_config_system_h

#include "h/core/bool.h"
#include "h/core/objects.h"
#include "h/container/list.h"
#include "h/external/external.h"

struct h_config_system_t;
typedef struct h_config_system_t h_config_system_t;

h_config_system_t *h_config_system_create(int argc, char *argv[],
    char *filename, h_core_objects_t *objects);

void h_config_system_destroy(h_config_system_t *system);

h_core_bool_t h_config_system_find(h_config_system_t *system, char *name);

h_core_bool_t h_config_system_find_as_double(h_config_system_t *system,
    char *name, double *value, double default_value);

h_core_bool_t h_config_system_find_as_string(h_config_system_t *system,
    char *name, char **value, char *default_value);

h_core_bool_t h_config_system_find_as_unsigned_long(h_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value);

h_core_bool_t h_config_system_find_as_unsigned_short(h_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value);

h_core_bool_t h_config_system_find_list_as_strings(h_config_system_t *system,
    char *name, h_container_list_t **list);

#endif
