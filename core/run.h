#ifndef h_core_run_h
#define h_core_run_h

#include "h/core/types.h"

enum h_core_run_style_t {
  H_CORE_RUN_STYLE_UNKNOWN,
  H_CORE_RUN_STYLE_ITERATIONS,
  H_CORE_RUN_STYLE_WALL_TIME,
  H_CORE_RUN_STYLE_GOAL_REACHED,
  H_CORE_RUN_STYLE_INFINITE
};
typedef enum h_core_run_style_t h_core_run_style_t;

struct h_core_run_t;
typedef struct h_core_run_t h_core_run_t;

void h_core_run_consider_goal_reached(h_core_run_t *run);

h_core_run_t *h_core_run_create(h_core_run_style_t run_style);

void h_core_run_destroy(h_core_run_t *run);

void h_core_run_increment_iterations(h_core_run_t *run);

h_core_bool_t h_core_run_is_complete(h_core_run_t *run);

void h_core_run_reset(h_core_run_t *run);

void h_core_run_set_max_iterations(h_core_run_t *run,
    unsigned long max_iterations);

void h_core_run_set_wall_time_expiration_seconds(h_core_run_t *run,
    unsigned long seconds);

#endif
