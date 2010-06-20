#ifndef h_core_days_hours_minutes_seconds_h
#define h_core_days_hours_minutes_seconds_h

struct h_core_days_hours_minutes_seconds_t {
  unsigned short days;
  unsigned short hours;
  unsigned short minutes;
  unsigned short seconds;
};
typedef struct h_core_days_hours_minutes_seconds_t
h_core_days_hours_minutes_seconds_t;

char *create_string_from_days_hours_minutes_seconds
(h_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
