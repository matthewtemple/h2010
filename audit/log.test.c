#include "h/audit/log.h"
#include "h/external/external.h"
#include "h/file/basic.h"

int main(int argc, char *argv[])
{
  h_audit_log_t *log;
  h_file_basic_t *disk_file_a;
  h_file_basic_t *disk_file_b;
  FILE *disk_file_a_fp;
  FILE *disk_file_b_fp;

  disk_file_a = h_file_basic_create("audit/a.log",
      H_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_a) {
    h_core_trace_exit("h_file_basic_create");
  }
  disk_file_a_fp = h_file_basic_get_file(disk_file_a);

  log = h_audit_log_create(disk_file_a_fp);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  h_audit_log_enter(log, "haud", "::int=%d::double=%f::string=%s::", 11111,
      2.2222, "three");

  disk_file_b = h_file_basic_create("audit/b.log",
      H_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_b) {
    h_core_trace_exit("h_audit_file_create");
  }
  disk_file_b_fp = h_file_basic_get_file(disk_file_b);

  if (!h_audit_log_add_file(log, disk_file_b_fp)) {
    h_core_trace_exit("h_audit_log_add_file");
  }

  h_audit_log_enter(log, "haud", "this should be in both %s.log and %s.log",
      "a", "b");

  if (!h_audit_log_add_file(log, stdout)) {
    h_core_trace_exit("h_audit_log_add_file");
  }

  h_audit_log_enter(log, "haud", "word");

  h_audit_log_enter(log, "haud", "The quick brown fox jumps over the lazy dog."
      "  The quick brown fox jumps over the lazy dog.");

  h_audit_log_destroy(log);
  h_file_basic_destroy(disk_file_a);
  h_file_basic_destroy(disk_file_b);

  return 0;
}
