#include "h/core/period.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_core_period_t {
  unsigned long seconds;
  time_t next_time_to_do_it;
};

h_core_period_t *h_core_period_create(unsigned long seconds)
{
  h_core_period_t *period;

  period = malloc(sizeof *period);
  if (period) {
    period->seconds = seconds;
    period->next_time_to_do_it = time(NULL);
  } else {
    h_core_trace("malloc() failed");
  }

  return period;
}

void h_core_period_destroy(h_core_period_t *period)
{
  free(period);
}

h_core_bool_t h_core_period_once(h_core_period_t *period)
{
  assert(period);
  h_core_bool_t do_it_this_time;
  time_t current_time;

  if (period->seconds != 0) {
    current_time = time(NULL);

    if (current_time >= period->next_time_to_do_it) {
      period->next_time_to_do_it = current_time + period->seconds + 0;
      do_it_this_time = h_core_bool_true;
    } else {
      do_it_this_time = h_core_bool_false;
    }
  } else {
    do_it_this_time = h_core_bool_true;
  }

  return do_it_this_time;
}
