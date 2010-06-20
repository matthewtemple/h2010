#ifndef h_file_mode_h
#define h_file_mode_h

#include "h/external/external.h"

enum h_file_mode_t {
  H_FILE_MODE_OPEN_FOR_READ = 0,
  H_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE = 1,
  H_FILE_MODE_OPEN_OR_CREATE_FOR_WRITE_AT_END = 2,
  H_FILE_MODE_OPEN_FOR_READ_AND_WRITE = 3,
  H_FILE_MODE_TRUNCATE_OR_CREATE_FOR_READ_AND_WRITE = 4,
  H_FILE_MODE_OPEN_OR_CREATE_FOR_READ_AND_WRITE_AT_END = 5
};
typedef enum h_file_mode_t h_file_mode_t;

#endif
