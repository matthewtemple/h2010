#include "h/config/tools.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/psql/result.h"
#include "h/psql/system.h"

int main(int argc, char *argv[])
{
  h_config_disable_if_running_batch_tests(argc, argv);

  h_psql_system_t *h_psql_system;
  h_psql_system_create_error_t h_psql_system_create_error;
  h_psql_system_query_error_t h_psql_system_query_error;
  char *h_psql_system_query_error_text;
  h_psql_result_t *h_psql_result;
  char *sql;
  h_container_map_t *row;
  h_psql_system_execute_error_t h_psql_system_execute_error;
  char *h_psql_system_execute_error_text;
  h_audit_log_t *log;
  unsigned long connection_count;

  connection_count = 1;

  log = h_audit_log_create(stdout);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  h_psql_system = h_psql_system_create("localhost", "h", "h", "h",
      connection_count, log, &h_psql_system_create_error);
  if (!h_psql_system) {
    switch (h_psql_system_create_error) {
      case h_psql_system_create_error_connect_failed:
        h_core_trace_exit("connect failed");
        break;
      case h_psql_system_create_error_unknown:
      default:
        h_core_trace_exit("h_psql_system_create");
        break;
    }
  }

  sql = h_core_string_append(NULL, "select * from thing where id > 1");
  if (!sql) {
    h_core_trace_exit("h_core_string_append");
  }
  h_psql_result = h_psql_system_query(h_psql_system, sql,
      &h_psql_system_query_error, &h_psql_system_query_error_text);
  if (!h_psql_result) {
    h_core_trace_exit("%s", h_psql_system_query_error_text);
  }

  h_psql_result_iterate_start(h_psql_result);
  while ((row = h_psql_result_iterate_next(h_psql_result))) {
    printf("id=%s name=%s\n", (char *) h_container_map_find(row, "id"),
        (char *) h_container_map_find(row, "name"));
  }

  h_psql_result_destroy(h_psql_result);
  free(sql);

  sql = h_core_string_append(NULL,
      "insert into thing (id, name) values (4, 'd')");
  if (!h_psql_system_execute(h_psql_system, sql, &h_psql_system_execute_error,
          &h_psql_system_execute_error_text)) {
    h_core_trace_exit("%s", h_psql_system_execute_error_text);
  }
  free(sql);

  h_psql_system_destroy(h_psql_system);
  h_audit_log_destroy(log);

  return 0;
}
