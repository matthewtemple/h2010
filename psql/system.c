#include "h/core/constants.h"
#include "h/core/objectey.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/psql/system.h"

#define LOCK_CONNECTION_SLEEP_MICROSECONDS H_CORE_STANDARD_SLEEP_MICROSECONDS
#define MAX_CONNECTION_COUNT 32

struct h_psql_system_t {
  PGconn *connections[MAX_CONNECTION_COUNT];
  pthread_mutex_t connection_mutexes[MAX_CONNECTION_COUNT];
  unsigned short connection_count;

  h_core_objectey_t string_objectey;
  char *db_host;
  char *db_name;
  h_audit_log_t *log;
};

static unsigned short lock_connection(h_psql_system_t *system);

static void unlock_connection(h_psql_system_t *system,
    unsigned short connection_index);

unsigned short lock_connection(h_psql_system_t *system)
{
  assert(system);
  unsigned short connection_index;
  h_core_bool_t got_one;
  unsigned long failed_tries;

  got_one = h_core_bool_false;
  failed_tries = 0;

  do {
    connection_index = random() % system->connection_count;
    if (0 == pthread_mutex_trylock
        (system->connection_mutexes + connection_index)) {
      got_one = h_core_bool_true;
    } else {
      failed_tries++;
      if (failed_tries > (system->connection_count * 2)) {
        usleep(LOCK_CONNECTION_SLEEP_MICROSECONDS);
      }
    }
  } while (!got_one);

  return connection_index;
}

h_psql_system_t *h_psql_system_create(const char *db_host, const char *db_name,
    const char *db_user, const char *db_password,
    unsigned short connection_count, h_audit_log_t *log,
    h_psql_system_create_error_t *error)
{
  assert(db_host);
  assert(db_name);
  assert(db_user);
  assert(db_password);
  assert(connection_count >= 1);
  assert(log);
  assert(error);
  h_psql_system_t *system;
  ConnStatusType pq_status;
  unsigned short each_connection;
  h_core_bool_t so_far_so_good;
  h_core_bool_t connection_mutex_needs_destroy[MAX_CONNECTION_COUNT];
  char *connection_string;

  if (!PQisthreadsafe()) {
    h_audit_log_trace(log, "hpsq", "PQisthreadsafe");
  }

  if (connection_count > MAX_CONNECTION_COUNT) {
    connection_count = MAX_CONNECTION_COUNT;
  }

  system = malloc(sizeof *system);
  if (system) {
    h_core_objectey_init(&system->string_objectey, h_core_string_compare,
        h_core_string_copy, h_core_string_destroy,
        h_core_string_get_as_string);

    system->connection_count = connection_count;
    system->log = log;

    /*
      TODO: check return codes
    */
    system->db_host = strdup(db_host);
    system->db_name = strdup(db_name);

    for (each_connection = 0; each_connection < connection_count;
         each_connection++) {
      *(system->connections + each_connection) = NULL;
      *(connection_mutex_needs_destroy + each_connection) = h_core_bool_false;
    }

    so_far_so_good = h_core_bool_true;
    for (each_connection = 0; each_connection < connection_count;
         each_connection++) {
      if (0 == pthread_mutex_init
          (system->connection_mutexes + each_connection, NULL)) {
        *(connection_mutex_needs_destroy + each_connection) = h_core_bool_true;
      } else {
        so_far_so_good = h_core_bool_false;
        h_audit_log_trace(log, "hpsq", "pthread_mutex_init");
        break;
      }
    }

  } else {
    so_far_so_good = h_core_bool_false;
    *error = h_psql_system_create_error_unknown;
    h_audit_log_trace(log, "hpsq", "malloc");
  }

  if (so_far_so_good) {
    if (asprintf(&connection_string, "host=%s dbname=%s user=%s password=%s",
            db_host, db_name, db_user, db_password) != -1) {
      for (each_connection = 0; each_connection < connection_count;
           each_connection++) {
        *(system->connections + each_connection)
          = PQconnectdb(connection_string);
        pq_status = PQstatus(*(system->connections + each_connection));
        if (CONNECTION_OK != pq_status) {
          switch (pq_status) {
            case CONNECTION_BAD:
              *error = h_psql_system_create_error_connect_failed;
              break;
            default:
              *error = h_psql_system_create_error_unknown;
              break;
          }
          h_audit_log_trace(log, "hpsq", "PQsetdbLogin");
          PQfinish(*(system->connections + each_connection));
          *(system->connections + each_connection) = NULL;
          free(system);
          system = NULL;
          break;
        }
      }
      free(connection_string);
    } else {
      h_audit_log_trace(log, "hpsq", "asprintf");
    }
  }

  if (system && !so_far_so_good) {
    for (each_connection = 0; each_connection < connection_count;
         each_connection++) {
      if (*(connection_mutex_needs_destroy + each_connection)) {
        if (pthread_mutex_destroy
            (system->connection_mutexes + each_connection) != 0) {
          h_audit_log_trace(log, "hpsq", "pthread_mutex_destroy");
        }
      }
      if (*(system->connections + each_connection)) {
        PQfinish(*(system->connections + each_connection));
      }
    }
    free(system);
    system = NULL;
  }

  return system;
}

void h_psql_system_destroy(h_psql_system_t *system)
{
  assert(system);
  unsigned short each_connection;

  free(system->db_host);
  free(system->db_name);
  for (each_connection = 0; each_connection < system->connection_count;
       each_connection++) {
    if (pthread_mutex_destroy
        (system->connection_mutexes + each_connection) != 0) {
      h_audit_log_trace(system->log, "hpsq", "pthread_mutex_destroy");
    }
    PQfinish(*(system->connections + each_connection));
  }
  free(system);
}

h_core_bool_t h_psql_system_execute(h_psql_system_t *system, char *sql,
    h_psql_system_execute_error_t *error, char **error_text)
{
  assert(system);
  assert(sql);
  assert(error);
  assert(error_text);
  h_core_bool_t success;
  PGresult *pg_result;
  ExecStatusType exec_status;
  unsigned short connection_index;

  success = h_core_bool_false;

  connection_index = lock_connection(system);
  {
    pg_result = PQexec(*(system->connections + connection_index),
        (const char *) sql);
  }
  unlock_connection(system, connection_index);

  if (pg_result) {
    exec_status = PQresultStatus(pg_result);
    if (PGRES_COMMAND_OK == exec_status) {
      success = h_core_bool_true;
    } else {
      switch (exec_status) {
        case PGRES_BAD_RESPONSE:
          *error = h_psql_system_execute_error_bad_response;
          break;
        case PGRES_EMPTY_QUERY:
          *error = h_psql_system_execute_error_empty_query;
          break;
        case PGRES_FATAL_ERROR:
          *error = h_psql_system_execute_error_fatal_error;
          break;
        default:
          *error = h_psql_system_execute_error_unknown;
          break;
      }
      *error_text = PQresultErrorMessage(pg_result);
      h_audit_log_trace(system->log, "hpsq", "%s", *error_text);
    }
    PQclear(pg_result);
  } else {
    *error = h_psql_system_execute_error_unknown;
  }

  return success;
}

h_psql_result_t *h_psql_system_query(h_psql_system_t *system, char *sql,
    h_psql_system_query_error_t *error, char **error_text)
{
  assert(system);
  assert(sql);
  assert(error);
  assert(error_text);
  h_psql_result_t *result;
  PGresult *pg_result;
  ExecStatusType exec_status;
  h_psql_result_create_error_t h_psql_result_create_error;
  unsigned short connection_index;

  result = NULL;

  connection_index = lock_connection(system);
  {
    pg_result = PQexec(*(system->connections + connection_index),
        (const char *) sql);
  }
  unlock_connection(system, connection_index);

  if (pg_result) {
    exec_status = PQresultStatus(pg_result);
    if (PGRES_TUPLES_OK == exec_status) {
      result = h_psql_result_create(pg_result, &system->string_objectey,
          &h_psql_result_create_error);
      if (!result) {
        *error = h_psql_system_query_error_unknown;
        h_audit_log_trace(system->log, "hpsq", "h_psql_result_create");
      }
    } else {
      switch (exec_status) {
        case PGRES_BAD_RESPONSE:
          *error = h_psql_system_query_error_bad_response;
          break;
        case PGRES_EMPTY_QUERY:
          *error = h_psql_system_query_error_empty_query;
          break;
        case PGRES_FATAL_ERROR:
          *error = h_psql_system_query_error_fatal_error;
          break;
        default:
          *error = h_psql_system_query_error_unknown;
          break;
      }
      *error_text = PQresultErrorMessage(pg_result);
      h_audit_log_trace(system->log, "hpsq", "%s", *error_text);
    }
  } else {
    *error = h_psql_system_query_error_unknown;
  }

  return result;
}

h_core_bool_t h_psql_system_query_count(h_psql_system_t *system, char *sql,
    unsigned long *count, h_psql_system_query_count_error_t *error,
    char **error_text)
{
  assert(system);
  assert(sql);
  assert(count);
  assert(error);
  assert(error_text);
  h_core_bool_t success;
  h_psql_result_t *result;
  h_container_map_t *row;
  char *count_string;
  h_psql_system_query_error_t h_psql_system_query_error;

  success = h_core_bool_false;

  result = h_psql_system_query
    (system, sql, &h_psql_system_query_error, error_text);
  if (result) {
    row = h_psql_result_get_row(result, 0);
    count_string = h_container_map_find(row, "c");
    if (count_string) {
      success = h_core_bool_true;
      *count = atol(count_string);
    } else {
      *error = h_psql_system_query_count_error_no_c_column;
    }
    h_psql_result_destroy(result);
  } else {
    *error = h_psql_system_query_count_error_unknown;
  }

  return success;
}

h_core_bool_t h_psql_system_vacuum(h_psql_system_t *system,
    h_psql_system_vacuum_error_t *error)
{
  assert(system);
  assert(error);
  h_core_bool_t success;
  h_psql_system_execute_error_t h_psql_system_execute_error;
  char *h_psql_system_execute_error_text;

  h_audit_log_enter(system->log, "hpsq", "vacuum %s:%s", system->db_host,
      system->db_name);

  if (h_psql_system_execute(system, "vacuum", &h_psql_system_execute_error,
          &h_psql_system_execute_error_text)) {
    success = h_core_bool_true;
    h_audit_log_enter(system->log, "hpsq", "vacuum %s:%s complete",
        system->db_host, system->db_name);
  } else {
    success = h_core_bool_false;
    *error = h_psql_system_vacuum_error_unknown;
    h_audit_log_trace(system->log, "hpsq", "h_psql_system_execute");
  }

  return success;
}

void unlock_connection(h_psql_system_t *system,
    unsigned short connection_index)
{
  pthread_mutex_unlock(system->connection_mutexes + connection_index);
}
