#ifndef h_net_server_h
#define h_net_server_h

#include "h/audit/log.h"
#include "h/config/system.h"
#include "h/container/list.h"
#include "h/core/engine.h"
#include "h/core/message.h"
#include "h/core/messagey.h"
#include "h/core/types.h"
#include "h/net/enginey.h"
#include "h/net/ip_address.h"
#include "h/net/postey.h"
#include "h/net/server_stats.h"

#define H_NET_SERVER_MAX_ENGINES 64
#define H_NET_SERVER_NO_MAINTAIN_FUNCTION NULL
#define H_NET_SERVER_NO_CONFIG_SYSTEM NULL
#define H_NET_SERVER_DONT_CLOSE_UNRESPONSIVE_CLIENTS 0
#define H_NET_MESSAGE_TYPE_COUNT_NONE 0

h_net_server_t *h_net_server_create(const char *name, unsigned short min_port,
    unsigned short max_port, unsigned short max_threads,
    h_core_messagey_t *messagey, h_net_postey_t *postey,
    h_net_engine_get_name_f get_engine_name, h_config_system_t *config_system,
    h_audit_log_t *log);

void h_net_server_destroy(h_net_server_t *server);

h_core_bool_t h_net_server_engine_run_thread_quiesce_requested
(h_net_server_t *server, h_net_engine_id_t engine_id,
    unsigned short thread_index);

h_net_server_handle_message_f h_net_server_get_handler_for_message
(h_net_server_t *server, void *message_object);

h_config_system_t *h_net_server_get_config_system(h_net_server_t *server);

void h_net_server_get_stats(h_net_server_t *server,
    h_net_server_stats_t *server_stats);

void h_net_server_print_stats(h_net_server_t *server);

void h_net_server_process_messages(h_net_server_t *server,
    h_net_engine_id_t engine_id, unsigned short thread_index);

h_core_bool_t h_net_server_register_engine(h_net_server_t *server,
    h_net_engine_id_t engine_id, void *custom_server_context,
    h_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, h_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

void h_net_server_register_message_handler(h_net_server_t *server,
    h_net_engine_id_t engine_id, unsigned long message_type,
    h_net_server_handle_message_f message_handler);

h_core_bool_t h_net_server_send_message(h_net_server_t *server,
    void *message_object);

void h_net_server_set_unresponsive_client_time_seconds
(h_net_server_t *server, unsigned long unresponsive_client_time_seconds);

h_core_bool_t h_net_server_start(h_net_server_t *server);

h_container_list_t *h_net_server_take_unprocessed_messages
(h_net_server_t *server);

h_container_list_t *h_net_server_take_unsent_messages(h_net_server_t *server);

#endif
