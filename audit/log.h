#ifndef h_audit_log_h
#define h_audit_log_h

#include "h/core/bool.h"
#include "h/core/tools.h"
#include "h/external/external.h"

#define h_audit_log_trace(log, system, entry, ...) _h_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__);

#define h_audit_log_trace_exit(log, system, entry, ...) _h_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__); exit(44);

struct h_audit_log_t;
typedef struct h_audit_log_t h_audit_log_t;

h_core_bool_t h_audit_log_add_file(h_audit_log_t *log, FILE *file);

h_audit_log_t *h_audit_log_create(FILE *file);

void h_audit_log_destroy(h_audit_log_t *log);

h_core_bool_t h_audit_log_enter(h_audit_log_t *log, const char *system,
    const char *entry, ...);

h_core_bool_t h_audit_log_remove_file(h_audit_log_t *log, FILE *file);

h_core_bool_t _h_audit_log_trace(h_audit_log_t *log, const char *system,
    const char *entry, const char *file, unsigned long line, ...);

#endif
