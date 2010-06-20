#include "h/core/constants.h"
#include "h/core/days_hours_minutes_seconds.h"
#include "h/core/time.h"
#include "h/core/tools.h"
#include "h/external/external.h"

int main(int argc, char *argv[])
{
  h_core_time_t *time;
  h_core_days_hours_minutes_seconds_t days_hours_minutes_seconds;
  unsigned long seconds;
  char *time_string;

  for (seconds = 1000000; seconds < 1000010; seconds++) {
    time = h_core_time_create(seconds);
    if (time) {
      h_core_time_get_days_hours_minutes_seconds(time,
          &days_hours_minutes_seconds);
      time_string = create_string_from_days_hours_minutes_seconds
        (&days_hours_minutes_seconds);
      if (time_string) {
        printf("%lu => %s\n", seconds, time_string);
        assert(seconds == days_hours_minutes_seconds.seconds
            + (days_hours_minutes_seconds.minutes * H_CORE_SECONDS_PER_MINUTE)
            + (days_hours_minutes_seconds.hours * H_CORE_SECONDS_PER_HOUR)
            + (days_hours_minutes_seconds.days * H_CORE_SECONDS_PER_DAY));
        free(time_string);
      }
      h_core_time_destroy(time);
    } else {
      h_core_trace("h_core_time_create");
    }
  }

  return 0;
}
