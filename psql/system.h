#ifndef h_psql_system_h
#define h_psql_system_h

#include "h/audit/log.h"
#include "h/psql/result.h"
#include "h/psql/system_error.h"

struct h_psql_system_t;
typedef struct h_psql_system_t h_psql_system_t;

h_psql_system_t *h_psql_system_create(const char *db_host, const char *db_name,
    const char *db_user, const char *db_password,
    unsigned short connection_count, h_audit_log_t *log,
    h_psql_system_create_error_t *error);

void h_psql_system_destroy(h_psql_system_t *system);

h_core_bool_t h_psql_system_execute(h_psql_system_t *system, char *sql,
    h_psql_system_execute_error_t *error, char **error_text);

h_psql_result_t *h_psql_system_query(h_psql_system_t *system,
    char *sql, h_psql_system_query_error_t *error, char **error_text);

h_core_bool_t h_psql_system_query_count(h_psql_system_t *system,
    char *sql, unsigned long *count, h_psql_system_query_count_error_t *error,
    char **error_text);

h_core_bool_t h_psql_system_vacuum(h_psql_system_t *system,
    h_psql_system_vacuum_error_t *error);

#endif
