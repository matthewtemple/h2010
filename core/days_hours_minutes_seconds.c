#include "h/core/days_hours_minutes_seconds.h"
#include "h/core/tools.h"
#include "h/core/types.h"
#include "h/external/external.h"

char *create_string_from_days_hours_minutes_seconds
(h_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds)
{
  assert(days_hours_minutes_seconds);
  char *string;
  int asprintf_result;

  asprintf_result = asprintf(&string, "%02i:%02i:%02i:%02i",
      days_hours_minutes_seconds->days, days_hours_minutes_seconds->hours,
      days_hours_minutes_seconds->minutes,
      days_hours_minutes_seconds->seconds);
  if (-1 == asprintf_result) {
    string = NULL;
    h_core_trace("asprintf");
  }

  return string;
}
