#ifndef h_config_file_h
#define h_config_file_h

#include "h/container/list.h"

struct h_config_file_t;
typedef struct h_config_file_t h_config_file_t;

h_config_file_t *h_config_file_create(char *filename);

void h_config_file_destroy(h_config_file_t *conf);

h_core_bool_t h_config_file_find(h_config_file_t *file, char *name);

h_core_bool_t h_config_file_find_as_double(h_config_file_t *file,
    char *name, double *value, double default_value);

h_core_bool_t h_config_file_find_as_string(h_config_file_t *file, char *name,
    char **value, char *default_value);

h_core_bool_t h_config_file_find_as_unsigned_long(h_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value);

h_core_bool_t h_config_file_find_as_unsigned_short(h_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value);

h_core_bool_t h_config_file_find_list_as_strings(h_config_file_t *file,
    char *name, h_container_list_t **list);

#endif
