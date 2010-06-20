#include "h/core/constants.h"
#include "h/core/message.h"
#include "h/core/messagey.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/client.h"
#include "h/net/node.h"
#include "h/net/post.h"
#include "h/net/postey.h"
#include "h/net/server.h"
#include "h/net/starclient.h"

struct h_net_node_t {
  h_net_server_t *server;
  h_core_messagey_t messagey;
  h_net_postey_t postey;

  h_net_starclient_t *starclient;

  pthread_t node_thread;

  h_audit_log_t *log;
};

static void h_net_node_create_rollback(h_net_node_t *node);

static void *node_thread(void *node_object);

h_net_node_t *h_net_node_create(char *node_ip, unsigned short node_min_port,
    unsigned short node_max_port, unsigned short node_max_threads,
    h_container_list_t *peer_node_ips, unsigned short peer_node_port_min,
    unsigned short peer_node_port_max, h_audit_log_t *log)
{
  assert(peer_node_ips);
  h_net_node_t *node;
  h_core_bool_t so_far_so_good;

  node = malloc(sizeof *node);
  if (node) {
    h_core_messagey_init(&node->messagey, h_core_message_destroy,
        h_core_message_get_client_socket,
        h_core_message_get_engine_id, h_core_message_get_type);
    h_net_postey_init(&node->postey, h_net_post_compare,
        h_net_post_create, h_net_post_create_decoy,
        h_net_post_destroy, h_net_post_destroy_decoy,
        h_net_post_get_last_receive_activity_time,
        h_net_post_get_socket, h_net_post_get_stats,
        h_net_post_receive_message, h_net_post_receive_messages,
        h_net_post_send_message, h_net_post_send_messages,
        h_net_post_is_socket_closed);
    node->log = log;
    so_far_so_good = h_core_bool_true;
  } else {
    h_core_trace("malloc");
    so_far_so_good = h_core_bool_false;
  }

  if (so_far_so_good) {
    node->server = h_net_server_create("standard", node_min_port,
        node_max_port, node_max_threads, &node->messagey, &node->postey,
        H_NET_ENGINE_NO_GET_NAME_FUNCTION, H_NET_SERVER_NO_CONFIG_SYSTEM, log);
    if (node->server) {
      so_far_so_good = h_core_bool_true;
    } else {
      h_core_trace_exit("h_net_server_create failed()");
      so_far_so_good = h_core_bool_false;
    }
  }

  if (so_far_so_good) {
    node->starclient = h_net_starclient_create(peer_node_ips,
        peer_node_port_min, peer_node_port_max, node_ip, node_min_port,
        node_max_port, NULL, log);
    if (node->starclient) {
      so_far_so_good = h_core_bool_true;
    } else {
      h_core_trace_exit("h_net_starclient_create() failed()");
      so_far_so_good = h_core_bool_false;
    }
  }

  if (!so_far_so_good && node) {
    h_net_node_create_rollback(node);
    node = NULL;
  }

  return node;
}

void h_net_node_create_rollback(h_net_node_t *node)
{
  assert(node);

  if (node->server) {
    h_core_request_stop();
    h_net_server_destroy(node->server);
  }
  if (node->starclient) {
    h_net_starclient_destroy(node->starclient);
  }
  free(node);
}

void h_net_node_destroy(h_net_node_t *node)
{
  assert(node);

  /*
    TODO: take unprocessed and unsent messages from the server and reroute
    them to another node
  */

  if (0 != pthread_join(node->node_thread, NULL)) {
    h_core_trace("pthread_join");
  }
  h_net_server_destroy(node->server);
  h_net_starclient_destroy(node->starclient);
  free(node);
}

h_net_server_t *h_net_node_get_server(h_net_node_t *node)
{
  return node->server;
}

void h_net_node_get_server_stats(h_net_node_t *node,
    h_net_server_stats_t *server_stats)
{
  h_net_server_get_stats(node->server, server_stats);
}

h_net_starclient_t *h_net_node_get_starclient(h_net_node_t *node)
{
  return node->starclient;
}

h_core_bool_t h_net_node_register_engine(h_net_node_t *node,
    h_net_engine_id_t engine_id, void *custom_server_context,
    h_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, h_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  return h_net_server_register_engine(node->server, engine_id,
      custom_server_context, enginey, min_run_threads, max_run_threads,
      maintain_schedule, message_type_count);
}

void h_net_node_register_message_handler(h_net_node_t *node,
    h_net_engine_id_t engine_id, unsigned long message_type,
    h_net_server_handle_message_f message_handler)
{
  h_net_server_register_message_handler(node->server, engine_id, message_type,
      message_handler);
}

h_core_bool_t h_net_node_send_message_to_all_peers(h_net_node_t *node,
    h_core_message_t *message)
{
  return h_net_starclient_send_message_to_all_arms(node->starclient, message);
}

h_core_bool_t h_net_node_send_message_to_any_peer(h_net_node_t *node,
    h_core_message_t *message)
{
  return h_net_starclient_send_message_to_any_arm(node->starclient, message);
}

h_core_bool_t h_net_node_send_message_to_client(h_net_node_t *node,
    h_core_message_t *message)
{
  return h_net_server_send_message(node->server, message);
}

void h_net_node_set_server_unresponsive_client_time_seconds(h_net_node_t *node,
    unsigned long unresponsive_client_time_seconds)
{
  h_net_server_set_unresponsive_client_time_seconds
    (node->server, unresponsive_client_time_seconds);
}

h_core_bool_t h_net_node_start(h_net_node_t *node)
{
  assert(node);
  h_core_bool_t success;

  if (h_net_server_start(node->server)) {
    h_net_starclient_connect(node->starclient);
    if (0 == pthread_create(&node->node_thread, NULL, node_thread, node)) {
      success = h_core_bool_true;
    } else {
      h_core_trace("pthread_create");
      success = h_core_bool_false;
    }
  } else {
    success = h_core_bool_false;
    h_core_request_stop();
  }

  return success;
}

void *node_thread(void *node_object)
{
  assert(node_object);
  h_net_node_t *node;

  node = node_object;

  while (!h_core_stop_requested) {
    h_net_starclient_process_messages(node->starclient);
    usleep(H_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  return NULL;
}
