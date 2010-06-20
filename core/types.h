#ifndef h_core_types_h
#define h_core_types_h

#include "h/core/object.h"

enum h_core_type_t {
  H_CORE_TYPE_UNKNOWN = 0,
  H_CORE_TYPE_BIT = 1,
  H_CORE_TYPE_H_CORE_BOOL_T = 2,
  H_CORE_TYPE_CHAR = 3,
  H_CORE_TYPE_DOUBLE = 4,
  H_CORE_TYPE_ENUM = 5,
  H_CORE_TYPE_LONG = 6,
  H_CORE_TYPE_SHORT = 7,
  H_CORE_TYPE_STRING = 8,
  H_CORE_TYPE_UNSIGNED_CHAR = 9,
  H_CORE_TYPE_UNSIGNED_LONG = 10,
  H_CORE_TYPE_UNSIGNED_SHORT = 11,
};
typedef enum h_core_type_t h_core_type_t;

typedef void *(*thread_f)(void *thread_context);

#define H_CORE_TIME_STRING_LENGTH 15
typedef char h_core_time_string_t[H_CORE_TIME_STRING_LENGTH + 1];

#endif
