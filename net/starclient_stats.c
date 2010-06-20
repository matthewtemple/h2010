#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/starclient_stats.h"

h_core_bool_t h_net_starclient_add_stats_to_message
(h_net_starclient_stats_t *starclient_stats, h_core_message_t *message)
{
  assert(message);
  assert(starclient_stats);
  h_core_bool_t success;

  if (h_core_message_add_long
      (message, &starclient_stats->connected_server_count)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

h_net_starclient_stats_t *h_net_starclient_create_stats_from_message
(h_core_message_t *message)
{
  assert(message);
  h_net_starclient_stats_t *starclient_stats;

  starclient_stats = malloc(sizeof *starclient_stats);
  if (starclient_stats) {
    starclient_stats->connected_server_count
      = h_core_message_take_long_value(message);
  } else {
    h_core_trace("malloc");
  }

  return starclient_stats;
}
