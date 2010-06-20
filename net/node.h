#ifndef h_net_node_h
#define h_net_node_h

#include "h/core/message.h"
#include "h/net/server.h"
#include "h/net/starclient.h"

struct h_net_node_t;
typedef struct h_net_node_t h_net_node_t;

h_net_node_t *h_net_node_create(char *node_ip, unsigned short node_min_port,
    unsigned short node_max_port, unsigned short node_max_threads,
    h_container_list_t *peer_node_ips, unsigned short peer_node_port_min,
    unsigned short peer_node_port_max, h_audit_log_t *log);

void h_net_node_destroy(h_net_node_t *node);

h_net_server_t *h_net_node_get_server(h_net_node_t *node);

void h_net_node_get_server_stats(h_net_node_t *node,
    h_net_server_stats_t *server_stats);

h_net_starclient_t *h_net_node_get_starclient(h_net_node_t *node);

h_core_bool_t h_net_node_register_engine(h_net_node_t *node,
    h_net_engine_id_t engine_id, void *custom_server_context,
    h_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, h_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

void h_net_node_register_message_handler(h_net_node_t *node,
    h_net_engine_id_t engine_id, unsigned long message_type,
    h_net_server_handle_message_f message_handler);

h_core_bool_t h_net_node_send_message_to_all_peers(h_net_node_t *node,
    h_core_message_t *message);

h_core_bool_t h_net_node_send_message_to_any_peer(h_net_node_t *node,
    h_core_message_t *message);

h_core_bool_t h_net_node_send_message_to_client(h_net_node_t *node,
    h_core_message_t *message);

void h_net_node_set_server_unresponsive_client_time_seconds
(h_net_node_t *node, unsigned long unresponsive_client_time_seconds);

h_core_bool_t h_net_node_start(h_net_node_t *node);

#endif
