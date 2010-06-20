#include "h/core/run.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_core_run_t {
  h_core_run_style_t run_style;

  h_core_bool_t goal_reached;

  unsigned long iterations;
  unsigned long max_iterations;

  time_t start_time;
  unsigned long wall_time_expiration_seconds;
};

static void reset(h_core_run_t *run);

void h_core_run_consider_goal_reached(h_core_run_t *run)
{
  run->goal_reached = h_core_bool_true;
}

h_core_run_t *h_core_run_create(h_core_run_style_t run_style)
{
  h_core_run_t *run;

  run = malloc(sizeof *run);
  if (run) {
    run->run_style = run_style;
    reset(run);
  } else {
    h_core_trace("malloc");
  }

  return run;
}

void h_core_run_destroy(h_core_run_t *run)
{
  assert(run);
  free(run);
}

void h_core_run_increment_iterations(h_core_run_t *run)
{
  run->iterations++;
}

h_core_bool_t h_core_run_is_complete(h_core_run_t *run)
{
  assert(run);
  h_core_bool_t is_complete;
  time_t current_time;

  switch (run->run_style) {

    case H_CORE_RUN_STYLE_ITERATIONS:
      if (run->iterations >= run->max_iterations) {
        is_complete = h_core_bool_true;
      } else {
        is_complete = h_core_bool_false;
      }
      break;

    case H_CORE_RUN_STYLE_WALL_TIME:
      time(&current_time);
      if ((current_time - run->start_time)
          >= run->wall_time_expiration_seconds) {
        is_complete = h_core_bool_true;
      } else {
        is_complete = h_core_bool_false;
      }
      break;

    case H_CORE_RUN_STYLE_GOAL_REACHED:
      is_complete = run->goal_reached;
      break;

    case H_CORE_RUN_STYLE_INFINITE:
      is_complete = h_core_bool_false;
      break;

    case H_CORE_RUN_STYLE_UNKNOWN:
    default:
      is_complete = h_core_bool_false;
      break;
  }

  return is_complete;
}

void h_core_run_reset(h_core_run_t *run)
{
  reset(run);
}

void h_core_run_set_max_iterations(h_core_run_t *run,
    unsigned long max_iterations)
{
  run->max_iterations = max_iterations;
}

void h_core_run_set_wall_time_expiration_seconds(h_core_run_t *run,
    unsigned long seconds)
{
  run->wall_time_expiration_seconds = seconds;
}

void reset(h_core_run_t *run)
{
  assert(run);

  run->goal_reached = h_core_bool_false;
  run->iterations = 0;
  time(&run->start_time);
}
