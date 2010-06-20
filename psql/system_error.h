#ifndef h_psql_system_error_h
#define h_psql_system_error_h

enum h_psql_system_create_error_t {
  h_psql_system_create_error_unknown,
  h_psql_system_create_error_connect_failed,
};
typedef enum h_psql_system_create_error_t h_psql_system_create_error_t;

enum h_psql_system_execute_error_t {
  h_psql_system_execute_error_unknown,
  h_psql_system_execute_error_bad_response,
  h_psql_system_execute_error_empty_query,
  h_psql_system_execute_error_fatal_error,
};
typedef enum h_psql_system_execute_error_t
h_psql_system_execute_error_t;

enum h_psql_system_query_count_error_t {
  h_psql_system_query_count_error_unknown,
  h_psql_system_query_count_error_no_c_column,
};
typedef enum h_psql_system_query_count_error_t
h_psql_system_query_count_error_t;

enum h_psql_system_query_error_t {
  h_psql_system_query_error_unknown,
  h_psql_system_query_error_bad_response,
  h_psql_system_query_error_empty_query,
  h_psql_system_query_error_fatal_error,
};
typedef enum h_psql_system_query_error_t h_psql_system_query_error_t;

enum h_psql_system_vacuum_error_t {
  h_psql_system_vacuum_error_unknown,
};
typedef enum h_psql_system_vacuum_error_t h_psql_system_vacuum_error_t;

#endif
