#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/client.h"
#include "h/net/clientsocket.h"
#include "h/net/exchange.h"
#include "h/net/post.h"
#include "h/net/postey.h"

struct engine_container_t {
  h_net_engine_id_t engine_id;
  h_net_client_handle_message_f *message_handlers;
  unsigned long message_handlers_size;
};
typedef struct engine_container_t engine_container_t;

struct h_net_client_t {
  int socket;

  h_net_post_t *post;
  h_net_exchange_t *exchange;
  pthread_mutex_t messaging_mutex;

  void *custom_client_context;
  char *server_ip_address;
  unsigned short server_min_port;
  unsigned short server_max_port;
  unsigned short server_port;

  h_container_list_t *engines;
  engine_container_t *engines_array[H_NET_ENGINE_TYPE_COUNT];

  h_core_bool_t server_socket_closed;

  h_net_postey_t postey;
  h_net_engine_get_name_f get_engine_name;

  h_audit_log_t *log;
};

static h_core_bool_t connect_to_server(h_net_client_t *client);

static void destroy_engine_container(void *object);

static h_core_bool_t ensure_client_is_connected(h_net_client_t *client);

static h_core_bool_t handle_disconnect(h_net_client_t *client);

static h_core_message_t *receive_message(h_net_client_t *client);

h_core_bool_t connect_to_server(h_net_client_t *client)
{
  assert(client);
  h_core_bool_t success;
  unsigned short port;

  success = h_core_bool_false;

  for (port = client->server_min_port;
       (port <= client->server_max_port) && (!success); port++) {
    client->socket
      = h_net_clientsocket_create(client->server_ip_address, port);
    if (client->socket >= 0) {
      client->server_socket_closed = h_core_bool_false;
      client->server_port = port;

      h_audit_log_enter(client->log, "hnet",
          "client connected to server on port %i", port);

      pthread_mutex_lock(&client->messaging_mutex);
      {
        client->post = h_net_post_create(client->socket);
        if (client->post) {
          if (h_net_exchange_register_post
              (client->exchange, client->post)) {
            success = h_core_bool_true;
          } else {
            h_audit_log_trace(client->log, "hnet",
                "h_net_exchange_register_post");
            h_net_clientsocket_destroy(client->socket);
            client->server_socket_closed = h_core_bool_true;
            h_net_post_destroy(client->post);
            client->post = NULL;
          }
        } else {
          h_audit_log_trace(client->log, "hnet", "h_net_post_create");
          h_net_clientsocket_destroy(client->socket);
          client->server_socket_closed = h_core_bool_true;
        }
      }
      pthread_mutex_unlock(&client->messaging_mutex);

    } else {
      h_audit_log_enter(client->log, "hnet",
          "client could not connect to server on port %i", port);
    }
  }

  return success;
}

void destroy_engine_container(void *object)
{
  assert(object);
  engine_container_t *engine_container;

  engine_container = object;
  free(engine_container->message_handlers);
  free(engine_container);
}

h_core_bool_t ensure_client_is_connected(h_net_client_t *client)
{
  assert(client);
  h_core_bool_t connected;

  connected = h_core_bool_true;

  if (client->server_socket_closed) {
    connected = connect_to_server(client);
    if (connected) {
      h_audit_log_enter(client->log, "hnet", "reconnected to server");
    } else {
      h_audit_log_enter(client->log, "hnet", "could not reconnect to server");
    }
  }

  return connected;
}

h_core_bool_t handle_disconnect(h_net_client_t *client)
{
  assert(client);
  h_core_bool_t success;

  h_audit_log_enter(client->log, "hnet", "lost connection to server");

  if (h_net_exchange_unregister_post(client->exchange, client->socket)) {
    success = h_core_bool_true;
    h_net_post_destroy(client->post);
    client->post = NULL;
  } else {
    success = h_core_bool_false;
    h_audit_log_trace(client->log, "hnet", "h_net_exchange_unregister_post");
  }

  return success;
}

void *h_net_client_copy(void *client_object)
{
  h_core_trace_exit("TODO: implement");
  return NULL;
}

h_net_client_t *h_net_client_create(const char *server_ip_address,
    unsigned short server_min_port, unsigned short server_max_port,
    h_net_engine_get_name_f get_engine_name, void *custom_client_context,
    h_audit_log_t *log)
{
  assert(server_ip_address);
  h_net_client_t *client;
  h_core_bool_t success;
  unsigned long each_engine_id;
  h_core_bool_t messaging_mutex_needs_destroy;
  h_core_bool_t connected;

  messaging_mutex_needs_destroy = h_core_bool_false;
  connected = h_core_bool_false;

  client = malloc(sizeof *client);
  if (client) {
    client->log = log;
    client->post = NULL;
    client->get_engine_name = get_engine_name;
    client->custom_client_context = custom_client_context;
    client->server_min_port = server_min_port;
    client->server_max_port = server_max_port;
    client->server_socket_closed = h_core_bool_true;
    for (each_engine_id = 0; each_engine_id < H_NET_ENGINE_TYPE_COUNT;
         each_engine_id++) {
      *(client->engines_array + each_engine_id) = NULL;
    }
    h_net_postey_init(&client->postey, h_net_post_compare,
        h_net_post_create, h_net_post_create_decoy,
        h_net_post_destroy, h_net_post_destroy_decoy,
        h_net_post_get_last_receive_activity_time,
        h_net_post_get_socket, h_net_post_get_stats,
        h_net_post_receive_message, h_net_post_receive_messages,
        h_net_post_send_message, h_net_post_send_messages,
        h_net_post_is_socket_closed);
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
    h_audit_log_trace(client->log, "hnet", "malloc");
  }

  if (success) {
    client->server_ip_address = strdup(server_ip_address);
    if (!client->server_ip_address) {
      success = h_core_bool_false;
    }
  }

  if (success) {
    client->exchange = h_net_exchange_create(&client->postey);
    if (!client->exchange) {
      success = h_core_bool_false;
    }
  }

  if (success) {
    client->engines = h_container_list_create(H_CORE_NO_COMPARE_FUNCTION,
        H_CORE_NO_COPY_FUNCTION, destroy_engine_container);
    if (!client->engines) {
      success = h_core_bool_false;
    }
  }

  if (success) {
    if (0 == pthread_mutex_init(&client->messaging_mutex, NULL)) {
      messaging_mutex_needs_destroy = h_core_bool_true;
    } else {
      success = h_core_bool_false;
      h_audit_log_trace(client->log, "hnet", "pthread_mutex_init");
    }
  }

  if (success) {
    connected = connect_to_server(client);
    if (!connected) {
      success = h_core_bool_false;
      h_audit_log_trace(client->log, "hnet", "connect_to_server");
    }
  }

  if (!success && client) {
    if (connected) {
      h_net_clientsocket_destroy(client->socket);
    }
    if (client->exchange) {
      h_net_exchange_destroy(client->exchange);
    }
    if (client->server_ip_address) {
      free(client->server_ip_address);
    }
    if (client->engines) {
      h_container_list_destroy(client->engines);
    }
    if (messaging_mutex_needs_destroy) {
      pthread_mutex_destroy(&client->messaging_mutex);
    }
    free(client);
    client = NULL;
  }

  return client;
}

void h_net_client_destroy(void *client_object)
{
  assert(client_object);
  h_net_client_t *client;

  client = client_object;

  h_net_clientsocket_destroy(client->socket);
  if (client->post) {
    h_net_post_destroy(client->post);
  }
  h_net_exchange_destroy(client->exchange);
  free(client->server_ip_address);
  h_container_list_destroy(client->engines);
  pthread_mutex_destroy(&client->messaging_mutex);

  free(client);
}

char *h_net_client_get_as_string(void *client_object)
{
  h_core_trace_exit("TODO: implement");
  return NULL;
}

char *h_net_client_get_server_ip_address(h_net_client_t *client)
{
  return client->server_ip_address;
}

unsigned short h_net_client_get_server_port(h_net_client_t *client)
{
  return client->server_port;
}

int h_net_client_get_socket(h_net_client_t *client)
{
  return client->socket;
}

h_core_bool_t h_net_client_is_connected_to_server(h_net_client_t *client)
{
  return !client->server_socket_closed;
}

void h_net_client_print(void *client_object)
{
  h_core_trace_exit("TODO: implement");
}

void h_net_client_process_messages(h_net_client_t *client)
{
  h_core_message_t *message;
  unsigned long message_type;
  h_net_client_handle_message_f handler;
  engine_container_t *engine_container;
  h_net_engine_id_t engine_id;

  handler = NULL;

  if (ensure_client_is_connected(client)) {
    while ((message = receive_message(client))) {
      message_type = h_core_message_get_type(message);
      engine_id = h_core_message_get_engine_id(message);
      engine_container = *(client->engines_array + engine_id);
      if (message_type < engine_container->message_handlers_size) {
        handler = *(engine_container->message_handlers + message_type);
        if (handler) {
          (*handler)(client->custom_client_context, message);
        }
      }
      if (!handler) {
        printf("client has no handler for %s engine's message %lu\n",
            h_net_engine_get_name(engine_container->engine_id,
                client->get_engine_name),
            message_type);
      }
      h_core_message_destroy(message);
    }
  } else {
    h_audit_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }
}

h_core_bool_t h_net_client_register_engine(h_net_client_t *client,
    h_net_engine_id_t engine_id, unsigned long message_type_count)
{
  engine_container_t *engine_container;
  h_core_bool_t success;
  unsigned short each_handler;

  engine_container = malloc(sizeof *engine_container);
  if (engine_container) {
    engine_container->engine_id = engine_id;
    success = h_core_bool_true;
  } else {
    h_audit_log_trace(client->log, "hnet", "malloc");
    success = h_core_bool_false;
  }

  if (success) {
    engine_container->message_handlers_size = message_type_count;
    engine_container->message_handlers
      = malloc((sizeof *engine_container->message_handlers)
          * message_type_count);
    if (engine_container->message_handlers) {
      for (each_handler = 0; each_handler < message_type_count;
           each_handler++) {
        *(engine_container->message_handlers + each_handler) = NULL;
      }
      success = h_core_bool_true;
    } else {
      h_audit_log_trace(client->log, "hnet", "malloc");
      success = h_core_bool_false;
    }
  }

  if (success) {
    *(client->engines_array + engine_id) = engine_container;
    if (!h_container_list_add_last(client->engines, engine_container)) {
      success = h_core_bool_false;
    }
  }

  return success;
}

void h_net_client_register_message_handler(h_net_client_t *client,
    h_net_engine_id_t engine_id, unsigned long message_type,
    h_net_client_handle_message_f message_handler)
{
  assert(client);
  assert(message_handler);
  assert(client->engines_array[engine_id]);
  assert(message_type
      < client->engines_array[engine_id]->message_handlers_size);
  engine_container_t *engine_container;

  engine_container = client->engines_array[engine_id];
  *(engine_container->message_handlers + message_type) = message_handler;
}

h_core_bool_t h_net_client_send_message(h_net_client_t *client,
    h_core_message_t *message)
{
  h_core_bool_t success;

  if (ensure_client_is_connected(client)) {

    pthread_mutex_lock(&client->messaging_mutex);
    {
      if (h_net_post_send_message(client->post, message)) {
        success = h_core_bool_true;
        h_net_exchange_send_and_receive_messages(client->exchange);

        if (h_net_post_is_socket_closed(client->post)) {
          client->server_socket_closed = h_core_bool_true;
          if (!handle_disconnect(client)) {
            h_audit_log_trace(client->log, "hnet", "handle_disconnect");
          }
        }
      } else {
        success = h_core_bool_false;
        h_audit_log_trace(client->log, "hnet", "h_net_post_send_message");
      }
    }
    pthread_mutex_unlock(&client->messaging_mutex);

  } else {
    success = h_core_bool_false;
    h_audit_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }

  return success;
}

h_container_list_t *h_net_client_take_unsent_messages(h_net_client_t *client)
{
  assert(client);
  h_container_list_t *messages;

  pthread_mutex_lock(&client->messaging_mutex);
  messages = h_net_post_take_unsent_messages(client->post);
  pthread_mutex_unlock(&client->messaging_mutex);

  return messages;
}

h_core_message_t *receive_message(h_net_client_t *client)
{
  h_core_message_t *message;

  if (ensure_client_is_connected(client)) {

    pthread_mutex_lock(&client->messaging_mutex);
    {
      h_net_exchange_send_and_receive_messages(client->exchange);
      message = h_net_post_receive_message(client->post);

      if (h_net_post_is_socket_closed(client->post)) {
        client->server_socket_closed = h_core_bool_true;
        if (!handle_disconnect(client)) {
          h_audit_log_trace(client->log, "hnet", "handle_disconnect");
        }
      }
    }
    pthread_mutex_unlock(&client->messaging_mutex);

  } else {
    message = NULL;
    h_audit_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }

  return message;
}
