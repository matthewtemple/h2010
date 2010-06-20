#ifndef h_core_scratch_h
#define h_core_scratch_h

struct h_core_scratch_t;
typedef struct h_core_scratch_t h_core_scratch_t;

h_core_scratch_t *h_core_scratch_create();

void h_core_scratch_destroy(h_core_scratch_t *h_core_scratch);

#endif
