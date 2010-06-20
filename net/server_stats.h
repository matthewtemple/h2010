#ifndef h_net_server_stats_h
#define h_net_server_stats_h

#include "h/core/message.h"

struct h_net_server_stats_t {
  unsigned long engine_count;
  unsigned long server_send_message_failure_count;
  unsigned long engine_cant_handle_message_now_count;
  unsigned long engine_handled_message_count;
  unsigned long engine_cant_handle_message_count;
  unsigned long discarded_message_for_nonexistent_client_count;
  unsigned long discarded_message_for_unregistered_engine_count;
  unsigned long discarded_message_engine_inbox_add_failed_count;
  unsigned long uptime_seconds;
  unsigned long connected_client_count;
  unsigned short max_threads;
  unsigned short thread_count;
};
typedef struct h_net_server_stats_t h_net_server_stats_t;

h_core_bool_t h_net_server_add_stats_to_message
(h_net_server_stats_t *server_stats, h_core_message_t *message);

h_net_server_stats_t *h_net_server_create_stats_from_message
(h_core_message_t *message);

void h_net_server_stats_init(h_net_server_stats_t *server_stats);

#endif
