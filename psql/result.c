#include "h/container/array.h"
#include "h/container/map.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/psql/result.h"

struct h_psql_result_t {
  h_container_array_t *rows;
  PGresult *pg_result;
};

h_psql_result_t *h_psql_result_create(PGresult *pg_result,
    h_core_objectey_t *string_objectey, h_psql_result_create_error_t *error)
{
  assert(pg_result);
  h_psql_result_t *result;
  int row_count;
  int field_count;
  int each_row;
  int each_column;
  char *field_name;
  char *field_value;
  h_container_map_t *map;
  h_core_bool_t so_far_so_good;

  result = malloc(sizeof *result);
  if (result) {
    result->pg_result = pg_result;
    row_count = PQntuples(pg_result);
    result->rows = h_container_array_create(row_count, h_container_map_compare,
        h_container_map_copy, h_container_map_destroy);
    if (result->rows) {
      field_count = PQnfields(pg_result);
      so_far_so_good = h_core_bool_true;
      for (each_row = 0; each_row < row_count; each_row++) {
        map = h_container_map_create(string_objectey, string_objectey,
            H_CONTAINER_MAP_DONT_DESTROY);
        if (map) {
          for (each_column = 0; each_column < field_count; each_column++) {
            field_name = PQfname(pg_result, each_column);
            field_value = PQgetvalue(pg_result, each_row, each_column);
            if (h_container_map_add(map, field_name, field_value)) {
              h_container_array_add(result->rows, each_row, map);
            } else {
              *error = h_psql_result_create_error_h_container_map_add_failed;
              so_far_so_good = h_core_bool_false;
              h_container_map_destroy(map);
              break;
            }
          }
        } else {
          *error = h_psql_result_create_error_h_container_map_create_failed;
          so_far_so_good = h_core_bool_false;
          break;
        }
      }
      if (!so_far_so_good) {
        free(result);
        result = NULL;
      }
    } else {
      *error = h_psql_result_create_error_h_container_array_create_failed;
    }
  } else {
    *error = h_psql_result_create_error_malloc_failed;
  }

  return result;
}

void h_psql_result_destroy(h_psql_result_t *result)
{
  h_container_array_destroy(result->rows);
  PQclear(result->pg_result);
  free(result);
}

h_container_map_t *h_psql_result_get_row(h_psql_result_t *result,
    unsigned long row_index)
{
  return h_container_array_find(result->rows, row_index);
}

unsigned long h_psql_result_get_row_count(h_psql_result_t *result)
{
  return h_container_array_get_size(result->rows);
}

h_container_map_t *h_psql_result_iterate_next(h_psql_result_t *result)
{
  return h_container_array_iterate_next(result->rows);
}

void h_psql_result_iterate_start(h_psql_result_t *result)
{
  h_container_array_iterate_start(result->rows);
}
