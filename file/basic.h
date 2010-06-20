#ifndef h_file_basic_h
#define h_file_basic_h

#include "h/container/list.h"
#include "h/core/types.h"
#include "h/external/external.h"
#include "h/file/mode.h"

struct h_file_basic_t;
typedef struct h_file_basic_t h_file_basic_t;

h_file_basic_t *h_file_basic_create(const char *filename, h_file_mode_t mode);

void h_file_basic_destroy(h_file_basic_t *file);

h_core_bool_t h_file_basic_exists(const char *filename);

h_core_bool_t h_file_basic_get_as_blob(h_file_basic_t *file, char **blob,
    unsigned long *blob_size);

h_container_list_t *h_file_basic_get_as_line_list(h_file_basic_t *file);

char *h_file_basic_get_as_string(h_file_basic_t *file);

h_container_list_t *h_file_basic_get_as_token_list(h_file_basic_t *file,
    char *delimiters);

FILE *h_file_basic_get_file(h_file_basic_t *file);

h_core_bool_t h_file_basic_get_line_count(h_file_basic_t *file,
    unsigned long *line_count);

h_core_bool_t h_file_basic_write_char(h_file_basic_t *file, int c);

h_core_bool_t h_file_basic_write_string(h_file_basic_t *file, char *string);

#endif
