#ifndef h_net_engine_thread_h
#define h_net_engine_thread_h

struct h_net_engine_thread_t {
  void *engine_object;
  unsigned short thread_index;
};
typedef struct h_net_engine_thread_t h_net_engine_thread_t;

h_net_engine_thread_t *h_net_engine_create_thread(void *engine_object,
    unsigned short thread_index);

void h_net_engine_destroy_thread(h_net_engine_thread_t *engine_thread);

#endif
