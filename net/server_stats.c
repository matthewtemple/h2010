#include "h/core/long.h"
#include "h/core/message.h"
#include "h/core/short.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/server_stats.h"

h_core_bool_t h_net_server_add_stats_to_message
(h_net_server_stats_t *server_stats, h_core_message_t *message)
{
  assert(message);
  assert(server_stats);
  h_core_bool_t success;

  success = h_core_bool_true;

  if (!h_core_long_add_to_message(&server_stats->engine_count, message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message
      (&server_stats->server_send_message_failure_count, message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message
      (&server_stats->engine_handled_message_count, message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message
      (&server_stats->discarded_message_for_nonexistent_client_count,
          message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message
      (&server_stats->discarded_message_for_unregistered_engine_count,
          message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message
      (&server_stats->discarded_message_engine_inbox_add_failed_count,
          message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message(&server_stats->uptime_seconds, message)) {
    success = h_core_bool_false;
  }

  if (!h_core_long_add_to_message(&server_stats->connected_client_count,
          message)) {
    success = h_core_bool_false;
  }

  if (!h_core_short_add_to_message(&server_stats->max_threads, message)) {
    success = h_core_bool_false;
  }

  if (!h_core_short_add_to_message(&server_stats->thread_count, message)) {
    success = h_core_bool_false;
  }

  return success;
}

h_net_server_stats_t *h_net_server_create_stats_from_message
(h_core_message_t *message)
{
  assert(message);
  h_net_server_stats_t *server_stats;

  server_stats = malloc(sizeof *server_stats);
  if (server_stats) {
    server_stats->engine_count = h_core_message_take_long_value(message);

    server_stats->server_send_message_failure_count
      = h_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_now_count
      = h_core_message_take_long_value(message);

    server_stats->engine_handled_message_count
      = h_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_count
      = h_core_message_take_long_value(message);

    server_stats->discarded_message_for_nonexistent_client_count
      = h_core_message_take_long_value(message);

    server_stats->discarded_message_for_unregistered_engine_count
      = h_core_message_take_long_value(message);

    server_stats->discarded_message_engine_inbox_add_failed_count
      = h_core_message_take_long_value(message);

    server_stats->uptime_seconds
      = h_core_message_take_long_value (message);

    server_stats->connected_client_count
      = h_core_message_take_long_value(message);

    server_stats->max_threads = h_core_message_take_short_value(message);

    server_stats->thread_count = h_core_message_take_short_value(message);
  } else {
    h_core_trace("malloc");
  }

  return server_stats;
}

void h_net_server_stats_init(h_net_server_stats_t *server_stats)
{
  server_stats->server_send_message_failure_count = 0;
  server_stats->engine_cant_handle_message_now_count = 0;
  server_stats->engine_handled_message_count = 0;
  server_stats->engine_cant_handle_message_count = 0;
  server_stats->discarded_message_for_nonexistent_client_count = 0;
  server_stats->discarded_message_for_unregistered_engine_count = 0;
  server_stats->discarded_message_engine_inbox_add_failed_count = 0;
}
