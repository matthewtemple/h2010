#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/engine_thread.h"

h_net_engine_thread_t *h_net_engine_create_thread(void *engine_object,
    unsigned short thread_index)
{
  h_net_engine_thread_t *engine_thread;

  engine_thread = malloc(sizeof *engine_thread);
  if (engine_thread) {
    engine_thread->engine_object = engine_object;
    engine_thread->thread_index = thread_index;
  }

  return engine_thread;
}

void h_net_engine_destroy_thread(h_net_engine_thread_t *engine_thread)
{
  assert(engine_thread);
  free(engine_thread);
}
