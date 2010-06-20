#include "h/core/constants.h"
#include "h/core/time.h"
#include "h/core/tools.h"
#include "h/core/types.h"
#include "h/external/external.h"

struct h_core_time_t {
  unsigned long seconds;
};

h_core_time_t *h_core_time_create(unsigned long seconds)
{
  h_core_time_t *time;

  time = malloc(sizeof *time);
  if (time) {
    time->seconds = seconds;
  } else {
    h_core_trace("malloc");
  }

  return time;
}

void h_core_time_destroy(h_core_time_t *time)
{
  assert(time);
  free(time);
}

void h_core_time_get_days_hours_minutes_seconds(h_core_time_t *time,
    h_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds)
{
  assert(time);
  assert(days_hours_minutes_seconds);
  unsigned long seconds_left;

  seconds_left = time->seconds;

  days_hours_minutes_seconds->days = seconds_left / H_CORE_SECONDS_PER_DAY;
  seconds_left -= (days_hours_minutes_seconds->days
      * H_CORE_SECONDS_PER_DAY);

  days_hours_minutes_seconds->hours = seconds_left / H_CORE_SECONDS_PER_HOUR;
  seconds_left -= (days_hours_minutes_seconds->hours
      * H_CORE_SECONDS_PER_HOUR);

  days_hours_minutes_seconds->minutes = seconds_left
    / H_CORE_SECONDS_PER_MINUTE;
  seconds_left -= (days_hours_minutes_seconds->minutes
      * H_CORE_SECONDS_PER_MINUTE);

  days_hours_minutes_seconds->seconds = seconds_left;

}
