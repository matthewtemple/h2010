#ifndef h_core_directory_h
#define h_core_directory_h

#include "h/core/bool.h"
#include "h/external/external.h"

struct h_core_directory_t;
typedef struct h_core_directory_t h_core_directory_t;

h_core_directory_t *h_core_directory_create();

void h_core_directory_destroy(h_core_directory_t *h_core_directory);

h_core_bool_t h_core_directory_exists(char *directory_name);

h_core_bool_t h_core_directory_make(char *directory_name, mode_t mode);

#endif
