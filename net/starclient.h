#ifndef h_net_starclient_h
#define h_net_starclient_h

#include "h/audit/log.h"
#include "h/core/engine.h"
#include "h/net/client.h"
#include "h/net/starclient_stats.h"

struct h_net_starclient_t;
typedef struct h_net_starclient_t h_net_starclient_t;

h_core_bool_t h_net_starclient_connect(h_net_starclient_t *starclient);

h_net_starclient_t *h_net_starclient_create(h_container_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    h_audit_log_t *log);

void h_net_starclient_destroy(h_net_starclient_t *starclient);

h_net_client_t *h_net_starclient_get_client(h_net_starclient_t *starclient,
    int socket);

void h_net_starclient_get_stats(h_net_starclient_t *starclient,
    h_net_starclient_stats_t *starclient_stats);

void h_net_starclient_process_messages(h_net_starclient_t *starclient);

h_core_bool_t h_net_starclient_register_engine(h_net_starclient_t *starclient,
    h_net_engine_id_t engine_id, unsigned long message_type_count);

void h_net_starclient_register_message_handler(h_net_starclient_t *starclient,
    h_net_engine_id_t engine_id, unsigned long message_type,
    h_net_client_handle_message_f message_handler);

h_core_bool_t h_net_starclient_send_message_to_any_arm
(h_net_starclient_t *starclient, h_core_message_t *message);

h_core_bool_t h_net_starclient_send_message_to_all_arms
(h_net_starclient_t *starclient, h_core_message_t *message);

void h_net_starclient_set_unsent_messages_queue_size
(h_net_starclient_t *starclient, unsigned long queue_size);

h_core_bool_t h_net_starclient_star_available(h_net_starclient_t *starclient);

#endif
