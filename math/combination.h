#ifndef h_math_combination_h
#define h_math_combination_h

#include "h/container/set.h"

struct h_math_combination_t;
typedef struct h_math_combination_t h_math_combination_t;

h_math_combination_t *h_math_combination_create(h_container_set_t *set);

void h_math_combination_destroy(h_math_combination_t *combination);

h_container_set_t *h_math_combination_iterate_next
(h_math_combination_t *combination);

void h_math_combination_iterate_start(h_math_combination_t *combination);

#endif
