#ifndef h_net_client_h
#define h_net_client_h

#include "h/audit/log.h"
#include "h/container/list.h"
#include "h/core/message.h"
#include "h/core/messagey.h"
#include "h/net/postey.h"

struct h_net_client_t;
typedef struct h_net_client_t h_net_client_t;

typedef void (*h_net_client_handle_message_f)(void *custom_client_context,
    h_core_message_t *message);

void *h_net_client_copy(void *client_object);

h_net_client_t *h_net_client_create(const char *server_ip_address,
    unsigned short server_min_port, unsigned short server_max_port,
    h_net_engine_get_name_f get_engine_name, void *custom_client_context,
    h_audit_log_t *log);

void h_net_client_destroy(void *client_object);

char *h_net_client_get_as_string(void *client_object);

char *h_net_client_get_server_ip_address(h_net_client_t *client);

unsigned short h_net_client_get_server_port(h_net_client_t *client);

int h_net_client_get_socket(h_net_client_t *client);

h_core_bool_t h_net_client_is_connected_to_server(h_net_client_t *client);

void h_net_client_print(void *client_object);

void h_net_client_process_messages(h_net_client_t *client);

h_core_bool_t h_net_client_register_engine(h_net_client_t *client,
    h_net_engine_id_t engine_id, unsigned long message_type_count);

void h_net_client_register_message_handler(h_net_client_t *client,
    h_net_engine_id_t engine_id, unsigned long message_type,
    h_net_client_handle_message_f message_handler);

h_core_bool_t h_net_client_send_message(h_net_client_t *client,
    h_core_message_t *message);

h_container_list_t *h_net_client_take_unsent_messages(h_net_client_t *client);

#endif
