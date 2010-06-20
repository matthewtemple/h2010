#include "h/core/engine.h"
#include "h/external/external.h"

static char *engine_names[H_NET_ENGINE_TYPE_COUNT] = {
  "void",
  "test",
  "ping",
  "shell",
  "hyper",
};

char *h_net_engine_get_name(unsigned long engine_id,
    h_net_engine_get_name_f get_engine_name)
{
  char *engine_name;

  if (engine_id < H_NET_ENGINE_TYPE_COUNT) {
    engine_name = engine_names[engine_id];
  } else {
    if (get_engine_name) {
      engine_name = get_engine_name(engine_id);
    } else {
      engine_name = "[unknown]";
    }
  }

  return engine_name;
}
