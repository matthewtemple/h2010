#ifndef h_core_buffer_h
#define h_core_buffer_h

#include "h/core/bool.h"

struct h_core_buffer_t;
typedef struct h_core_buffer_t h_core_buffer_t;

h_core_bool_t h_core_buffer_append_char(h_core_buffer_t *buffer, char c);

h_core_bool_t h_core_buffer_append_string(h_core_buffer_t *buffer,
    char *string, unsigned long string_size);

void h_core_buffer_clear(h_core_buffer_t *buffer);

h_core_buffer_t *h_core_buffer_create(char *string, unsigned long string_size);

void h_core_buffer_destroy(h_core_buffer_t *buffer);

char *h_core_buffer_get_string(h_core_buffer_t *buffer,
    unsigned long *string_size);

char *h_core_buffer_take_string(h_core_buffer_t *buffer,
    unsigned long *string_size);

#endif
