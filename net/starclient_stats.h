#ifndef h_net_starclient_stats_h
#define h_net_starclient_stats_h

#include "h/core/message.h"

struct h_net_starclient_stats_t {
  unsigned long connected_server_count;
};
typedef struct h_net_starclient_stats_t h_net_starclient_stats_t;

h_core_bool_t h_net_starclient_add_stats_to_message
(h_net_starclient_stats_t *starclient_stats, h_core_message_t *message);

h_net_starclient_stats_t *h_net_starclient_create_stats_from_message
(h_core_message_t *message);

#endif
