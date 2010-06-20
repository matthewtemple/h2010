#ifndef h_core_time_h
#define h_core_time_h

#include "h/core/days_hours_minutes_seconds.h"

struct h_core_time_t;
typedef struct h_core_time_t h_core_time_t;

h_core_time_t *h_core_time_create(unsigned long seconds);

void h_core_time_destroy(h_core_time_t *time);

void h_core_time_get_days_hours_minutes_seconds(h_core_time_t *time,
    h_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
