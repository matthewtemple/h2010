#ifndef h_format_json_h
#define h_format_json_h

#include "h/external/external.h"

struct h_format_json_t;
typedef struct h_format_json_t h_format_json_t;

h_format_json_t *h_format_json_create();

void h_format_json_destroy(h_format_json_t *json);

#endif
