#ifndef h_psql_result_h
#define h_psql_result_h

#include "h/container/map.h"
#include "h/core/objectey.h"
#include "h/psql/result_error.h"

struct h_psql_result_t;
typedef struct h_psql_result_t h_psql_result_t;

h_psql_result_t *h_psql_result_create(PGresult *pg_result,
    h_core_objectey_t *string_objectey, h_psql_result_create_error_t *error);

void h_psql_result_destroy(h_psql_result_t *result);

h_container_map_t *h_psql_result_get_row(h_psql_result_t *result,
    unsigned long row_index);

unsigned long h_psql_result_get_row_count(h_psql_result_t *result);

h_container_map_t *h_psql_result_iterate_next(h_psql_result_t *result);

void h_psql_result_iterate_start(h_psql_result_t *result);

#endif
