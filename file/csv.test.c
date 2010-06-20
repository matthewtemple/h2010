#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/file/csv.h"

int main(int argc, char *argv[])
{
  h_file_csv_t *csv;
  unsigned long object_count;
  unsigned long field_count;
  unsigned long each_object;
  unsigned long each_field;
  char *value;

  csv = h_file_csv_create_extended("file/csv_test.csv", 1, 2);
  if (!csv) {
    h_core_trace_exit("h_file_csv_create");
  }

  object_count = h_file_csv_get_object_count(csv);
  field_count = h_file_csv_get_field_count(csv);

  for (each_field = 0; each_field < field_count; each_field++) {
    for (each_object = 0; each_object < object_count; each_object++) {
      value = h_file_csv_get_value_by_index_as_string(csv, each_object,
          each_field);
      printf("%s,", value);
    }
    printf("\n");
  }

  h_file_csv_destroy(csv);

  return 0;
}
