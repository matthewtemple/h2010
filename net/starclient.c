#include "h/container/set.h"
#include "h/core/nameobject.h"
#include "h/core/period.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/client.h"
#include "h/net/starclient.h"
#include "h/net/types.h"

/*
  TODO: do we need to remove messages from the unsent queue if they've been
  there too long?
*/

#define DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE 1024
#define MAINTAIN_PERIOD 8
#define MAX_CLIENTS 1024

struct message_handler_info_t {
  h_net_engine_id_t engine_id;
  unsigned long message_type;
  h_net_client_handle_message_f message_handler;
};
typedef struct message_handler_info_t message_handler_info_t;

struct h_net_starclient_t {
  h_container_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_min_port;
  unsigned short node_server_exclude_max_port;

  h_container_set_t *clients;
  h_container_list_t *client_list;

  h_net_maintain_t maintain_schedule;
  h_core_bool_t stop_requested;
  h_core_period_t *maintain_period;

  h_container_list_t *unsent_messages;
  unsigned long unsent_messages_queue_size;

  unsigned long last_star_arm_count;
  h_core_bool_t need_to_print_stats;

  long message_type_counts[H_NET_ENGINE_TYPE_COUNT];
  h_container_list_t *message_handler_info;

  void *custom_client_context;
  h_audit_log_t *log;
};

static h_core_bool_t client_connected(h_net_starclient_t *starclient,
    char *client_name);

static char *create_client_name(char *server_ip, unsigned short server_port);

static void establish_connection(h_net_starclient_t *starclient,
    char *client_name, char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context);

static h_core_bool_t exclude_ip_port_combination
(h_net_starclient_t *starclient, char *ip, unsigned short port);

static h_net_client_t *find_client(h_net_starclient_t *starclient,
    char *client_name);

static h_net_client_t *get_random_client(h_net_starclient_t *starclient);

static h_core_bool_t h_net_starclient_create_client_list
(h_net_starclient_t *starclient);

static h_core_bool_t h_net_starclient_create_clients
(h_net_starclient_t *starclient);

static h_core_bool_t h_net_starclient_create_maintain_period
(h_net_starclient_t *starclient);

static h_core_bool_t h_net_starclient_create_message_handler_info
(h_net_starclient_t *starclient);

static h_core_bool_t h_net_starclient_create_unsent_messages
(h_net_starclient_t *starclient);

static void h_net_starclient_create_rollback(h_net_starclient_t *starclient);

static void print_stats(h_net_starclient_t *starclient);

static h_core_bool_t put_messsage_in_unsent_queue
(h_net_starclient_t *starclient, h_core_message_t *message);

static void re_route_unsent_messages(h_net_starclient_t *starclient);

static void rebuild_client_list(h_net_starclient_t *starclient);

static h_core_bool_t register_engines_with_client
(h_net_starclient_t *starclient, h_net_client_t *client);

static void register_message_handlers_with_client
(h_net_starclient_t *starclient, h_net_client_t *client);

static void take_unsent_messages(h_net_starclient_t *starclient,
    h_net_client_t *client);

h_core_bool_t client_connected(h_net_starclient_t *starclient,
    char *client_name)
{
  assert(starclient);
  assert(client_name);
  h_core_bool_t connected;
  h_net_client_t *client;
  h_core_nameobject_t *nameclient_decoy;

  client = find_client(starclient, client_name);
  if (client) {
    if (h_net_client_is_connected_to_server(client)) {
      connected = h_core_bool_true;
    } else {
      connected = h_core_bool_false;
      take_unsent_messages(starclient, client);
      nameclient_decoy = h_core_nameobject_create_decoy(client_name);
      if (nameclient_decoy) {
        if (h_container_set_remove(starclient->clients, nameclient_decoy)) {
          printf("[star] lost connection with %s\n", client_name);
          starclient->need_to_print_stats = h_core_bool_true;
        } else {
          h_core_trace("this should never happen");
        }
        h_core_nameobject_destroy_decoy(nameclient_decoy);
      } else {
        h_core_trace("h_core_nameobject_create_decoy");
      }
    }
  } else {
    connected = h_core_bool_false;
  }

  return connected;
}

char *create_client_name(char *server_ip, unsigned short server_port)
{
  assert(server_ip);
  char *client_name;
  unsigned short server_ip_size;
  unsigned short server_port_string_size;
  unsigned short client_name_size;
  char *server_port_string;

  client_name = NULL;

  server_ip_size = strlen(server_ip);
  server_port_string = malloc(6);
  if (server_port_string) {
    if (snprintf(server_port_string, 6, "%i", server_port) >= 0) {
      server_port_string_size = strlen(server_port_string);
      client_name_size = server_ip_size + 1 + server_port_string_size;
      client_name = malloc(client_name_size + 1);
      if (client_name) {
        memcpy(client_name, server_ip, server_ip_size);
        memcpy(client_name + server_ip_size, ":", 1);
        memcpy(client_name + server_ip_size + 1, server_port_string,
            server_port_string_size);
        *(client_name + client_name_size) = '\0';
      } else {
        h_core_trace("malloc");
      }
    } else {
      h_core_trace("snprintf");
    }
  } else {
    h_core_trace("malloc");
  }
  free(server_port_string);

  return client_name;
}

void establish_connection(h_net_starclient_t *starclient, char *client_name,
    char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context)
{
  assert(starclient);
  assert(client_name);
  h_core_bool_t established_connection;
  h_net_client_t *client;
  h_core_nameobject_t *nameclient;

  established_connection = h_core_bool_false;

  client = h_net_client_create(server_ip, server_min_port, server_max_port,
      H_NET_ENGINE_NO_GET_NAME_FUNCTION, custom_client_context,
      starclient->log);
  if (client) {
    nameclient = h_core_nameobject_create(client_name, client,
        H_CORE_NO_COPY_FUNCTION, h_net_client_destroy,
        h_net_client_get_as_string);
    if (nameclient) {
      if (h_container_set_add(starclient->clients, nameclient)) {
        if (register_engines_with_client(starclient, client)) {
          register_message_handlers_with_client(starclient, client);
          printf("[star] connect to %s\n", client_name);
          starclient->need_to_print_stats = h_core_bool_true;
          established_connection = h_core_bool_true;
        } else {
          h_core_trace("register_engines_with_client");
        }
      } else {
        h_core_trace("h_container_set_add");
        h_core_nameobject_destroy(nameclient);
      }
    } else {
      h_core_trace("h_core_nameobject_create");
    }
  }
  if (!established_connection) {
    h_net_client_destroy(client);
  }
}

h_core_bool_t exclude_ip_port_combination(h_net_starclient_t *starclient,
    char *ip, unsigned short port)
{
  assert(starclient);
  assert(ip);
  assert(port);
  h_core_bool_t exclude;

  if (((port >= starclient->node_server_exclude_min_port)
          && (port <= starclient->node_server_exclude_max_port))
      && (0 == strcmp(ip, starclient->node_server_exclude_ip))) {
    exclude = h_core_bool_true;
  } else {
    exclude = h_core_bool_false;
  }

  return exclude;
}

h_net_client_t *find_client(h_net_starclient_t *starclient, char *client_name)
{
  assert(starclient);
  assert(client_name);
  h_core_nameobject_t *nameclient_decoy;
  h_core_nameobject_t *nameclient;
  h_net_client_t *client;

  nameclient_decoy = h_core_nameobject_create_decoy(client_name);
  if (nameclient_decoy) {
    nameclient = h_container_set_find(starclient->clients, nameclient_decoy);
    h_core_nameobject_destroy_decoy(nameclient_decoy);
    if (nameclient) {
      client = h_core_nameobject_get_object(nameclient);
    } else {
      client = NULL;
    }
  } else {
    h_core_trace_exit("h_core_nameobject_create_decoy");
    client = NULL;
  }

  return client;
}

h_net_client_t *get_random_client(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_nameobject_t *nameclient;
  h_net_client_t *client;

  nameclient = h_container_list_find_random(starclient->client_list);
  if (nameclient) {
    client = h_core_nameobject_get_object(nameclient);
  } else {
    client = NULL;
  }

  return client;
}

h_core_bool_t h_net_starclient_connect(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_bool_t connected_to_at_least_one;
  char *arm_ip;
  unsigned short port;
  char *client_name;
  unsigned long star_arm_count;

  h_container_list_iterate_start(starclient->star_arm_ips);
  while ((arm_ip = h_container_list_iterate_next(starclient->star_arm_ips))) {
    for (port = starclient->star_arm_port_min;
         port <= starclient->star_arm_port_max; port++) {
      if (!exclude_ip_port_combination(starclient, arm_ip, port)) {
        client_name = create_client_name(arm_ip, port);
        if (!client_connected(starclient, client_name)) {
          establish_connection(starclient, client_name, arm_ip, port, port,
              starclient->custom_client_context);
        }
        free(client_name);
        client_name = NULL;
      }
    }
  }

  star_arm_count = h_container_set_get_size(starclient->clients);
  if (star_arm_count != starclient->last_star_arm_count) {
    starclient->need_to_print_stats = h_core_bool_true;
  }

  if (h_container_set_get_size(starclient->clients) > 0) {
    connected_to_at_least_one = h_core_bool_true;
  } else {
    connected_to_at_least_one = h_core_bool_false;
  }

  return connected_to_at_least_one;
}

h_net_starclient_t *h_net_starclient_create(h_container_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    h_audit_log_t *log)
{
  assert(star_arm_ips);
  assert(log);
  h_net_starclient_t *starclient;
  h_core_bool_t so_far_so_good;
  unsigned long engine_id;

  starclient = malloc(sizeof *starclient);
  if (starclient) {
    starclient->log = log;
    starclient->stop_requested = h_core_bool_false;
    starclient->last_star_arm_count = 0;
    starclient->need_to_print_stats = h_core_bool_false;
    starclient->star_arm_ips = star_arm_ips;
    starclient->star_arm_port_min = star_arm_port_min;
    starclient->star_arm_port_max = star_arm_port_max;
    starclient->node_server_exclude_ip = node_server_exclude_ip;
    starclient->node_server_exclude_min_port = node_server_exclude_min_port;
    starclient->node_server_exclude_max_port = node_server_exclude_max_port;
    starclient->custom_client_context = custom_client_context;
    starclient->unsent_messages_queue_size
      = DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE;
    for (engine_id = 0; engine_id < H_NET_ENGINE_TYPE_COUNT; engine_id++) {
      *(starclient->message_type_counts + engine_id) = -1;
    }
    if (h_net_starclient_create_clients(starclient)) {
      so_far_so_good = h_core_bool_true;
    } else {
      so_far_so_good = h_core_bool_false;
    }
  } else {
    h_core_trace("malloc");
    so_far_so_good = h_core_bool_false;
  }

  if (so_far_so_good) {
    so_far_so_good = h_net_starclient_create_maintain_period(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = h_net_starclient_create_unsent_messages(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = h_net_starclient_create_client_list(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = h_net_starclient_create_message_handler_info(starclient);
  }

  if (!so_far_so_good && starclient) {
    h_net_starclient_create_rollback(starclient);
    starclient = NULL;
  }

  assert(!starclient
      || (
        starclient->clients && starclient->maintain_period
        && starclient->unsent_messages && starclient->client_list
        )
    );
  return starclient;
}

h_core_bool_t h_net_starclient_create_client_list
(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_bool_t success;

  starclient->client_list = h_container_list_create(h_core_nameobject_compare,
      h_core_nameobject_copy, H_CORE_NO_DESTROY_FUNCTION);
  if (starclient->client_list) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_net_starclient_create_clients(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_bool_t success;

  starclient->clients = h_container_set_create(h_core_nameobject_compare,
      h_core_nameobject_copy, h_core_nameobject_destroy);
  if (starclient->clients) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_net_starclient_create_maintain_period
(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_bool_t success;

  starclient->maintain_period = h_core_period_create(MAINTAIN_PERIOD);
  if (starclient->maintain_period) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
    h_core_trace("new");
  }

  return success;
}

h_core_bool_t h_net_starclient_create_message_handler_info
(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_bool_t success;

  starclient->message_handler_info = h_container_list_create
    (H_CORE_NO_COMPARE_FUNCTION, H_CORE_NO_COPY_FUNCTION, free);
  if (starclient->message_handler_info) {
    success = h_core_bool_true;
  } else {
    h_core_trace("h_container_list_create");
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_net_starclient_create_unsent_messages
(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_bool_t success;

  starclient->unsent_messages = h_container_list_create
    (H_CORE_NO_COMPARE_FUNCTION, h_core_message_copy,
        H_CORE_NO_DESTROY_FUNCTION);
  if (starclient->unsent_messages) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

void h_net_starclient_create_rollback(h_net_starclient_t *starclient)
{
  assert(starclient);

  if (starclient->clients) {
    h_container_set_destroy(starclient->clients);
  }
  if (starclient->maintain_period) {
    h_core_period_destroy(starclient->maintain_period);
  }
  if (starclient->unsent_messages) {
    h_container_list_destroy(starclient->unsent_messages);
  }
  if (starclient->client_list) {
    h_container_list_destroy(starclient->client_list);
  }
  if (starclient->message_handler_info) {
    h_container_list_destroy(starclient->message_handler_info);
  }
  free(starclient);
}

void h_net_starclient_destroy(h_net_starclient_t *starclient)
{
  assert(starclient);
  unsigned int unsent_message_count;

  unsent_message_count
    = h_container_list_get_size(starclient->unsent_messages);

  if (unsent_message_count > 0) {
    printf("[star] %u messages in unsent queue were lost\n",
        unsent_message_count);
  }

  h_container_set_destroy(starclient->clients);
  h_core_period_destroy(starclient->maintain_period);
  h_container_list_destroy(starclient->unsent_messages);
  h_container_list_destroy(starclient->client_list);
  h_container_list_destroy(starclient->message_handler_info);
  free(starclient);
}

h_net_client_t *h_net_starclient_get_client(h_net_starclient_t *starclient,
    int socket)
{
  assert(starclient);
  h_core_nameobject_t *nameclient;
  h_net_client_t *client;
  h_core_bool_t found_it;
  int each_socket;

  found_it = h_core_bool_false;
  client = NULL;

  h_container_list_iterate_start(starclient->client_list);
  while (!found_it && (nameclient = h_container_list_iterate_next
          (starclient->client_list))) {
    client = h_core_nameobject_get_object(nameclient);
    each_socket = h_net_client_get_socket(client);
    if (each_socket == socket) {
      found_it = h_core_bool_true;
    }
  }

  return client;
}

void h_net_starclient_get_stats(h_net_starclient_t *starclient,
    h_net_starclient_stats_t *starclient_stats)
{
  assert(starclient);
  assert(starclient_stats);

  starclient_stats->connected_server_count
    = h_container_set_get_size(starclient->clients);
}

void h_net_starclient_process_messages(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_nameobject_t *nameclient;
  h_net_client_t *client;

  h_container_set_iterate_start(starclient->clients);
  while ((nameclient = h_container_set_iterate_next(starclient->clients))) {
    client = h_core_nameobject_get_object(nameclient);
    h_net_client_process_messages(client);
  }

  if (h_core_period_once(starclient->maintain_period)) {
    h_net_starclient_connect(starclient);
    rebuild_client_list(starclient);
    re_route_unsent_messages(starclient);
    if (starclient->need_to_print_stats) {
      print_stats(starclient);
    }
  }
}

h_core_bool_t h_net_starclient_register_engine(h_net_starclient_t *starclient,
    h_net_engine_id_t engine_id, unsigned long message_type_count)
{
  assert(starclient);
  h_core_nameobject_t *nameclient;
  h_net_client_t *client;
  h_core_bool_t success;

  success = h_core_bool_true;

  *(starclient->message_type_counts + engine_id) = message_type_count;

  h_container_set_iterate_start(starclient->clients);
  while ((nameclient = h_container_set_iterate_next(starclient->clients))) {
    client = h_core_nameobject_get_object(nameclient);
    if (!h_net_client_register_engine(client, engine_id, message_type_count)) {
      success = h_core_bool_false;
    }
  }

  return success;
}

void h_net_starclient_register_message_handler(h_net_starclient_t *starclient,
    h_net_engine_id_t engine_id, unsigned long message_type,
    h_net_client_handle_message_f message_handler)
{
  assert(starclient);
  h_core_nameobject_t *nameclient;
  h_net_client_t *client;
  message_handler_info_t *message_handler_info;

  message_handler_info = malloc(sizeof *message_handler_info);
  if (message_handler_info) {
    message_handler_info->engine_id = engine_id;
    message_handler_info->message_type = message_type;
    message_handler_info->message_handler = message_handler;
    if (!h_container_list_add_last
        (starclient->message_handler_info, message_handler_info)) {
      h_core_trace("h_container_list_add_last");
      free(message_handler_info);
    }
  } else {
    h_core_trace("malloc");
  }

  h_container_set_iterate_start(starclient->clients);
  while ((nameclient = h_container_set_iterate_next(starclient->clients))) {
    client = h_core_nameobject_get_object(nameclient);
    h_net_client_register_message_handler
      (client, engine_id, message_type, message_handler);
  }
}

h_core_bool_t h_net_starclient_send_message_to_any_arm
(h_net_starclient_t *starclient, h_core_message_t *message)
{
  assert(starclient);
  assert(message);
  h_net_client_t *random_client;
  h_core_bool_t success;
  h_core_bool_t sent_to_client;

  random_client = get_random_client(starclient);
  if (random_client) {
    if (h_net_client_send_message(random_client, message)) {
      sent_to_client = h_core_bool_true;
    } else {
      sent_to_client = h_core_bool_false;
    }
  } else {
    sent_to_client = h_core_bool_false;
  }

  if (sent_to_client) {
    success = h_core_bool_true;
  } else {
    if (put_messsage_in_unsent_queue(starclient, message)) {
      success = h_core_bool_true;
    } else {
      printf("[star] unsent message queue is full, unable to send message\n");
      success = h_core_bool_false;
    }
  }

  return success;
}

h_core_bool_t h_net_starclient_send_message_to_all_arms
(h_net_starclient_t *starclient, h_core_message_t *message)
{
  assert(starclient);
  assert(message);
  h_core_bool_t success;
  h_core_nameobject_t *nameclient;
  h_net_client_t *client;
  h_core_message_t *message_copy;

  success = h_core_bool_true;

  h_container_set_iterate_start(starclient->clients);
  while ((nameclient = h_container_set_iterate_next(starclient->clients))) {
    client = h_core_nameobject_get_object(nameclient);
    message_copy = h_core_message_copy(message);
    if (message_copy) {
      if (!h_net_client_send_message(client, message_copy)) {
        success = h_core_bool_false;
        h_core_message_destroy(message_copy);
      }
    } else {
      success = h_core_bool_false;
      h_core_trace("h_core_message_copy");
    }
  }

  h_core_message_destroy(message);

  return success;
}

void h_net_starclient_set_unsent_messages_queue_size
(h_net_starclient_t *starclient, unsigned long queue_size)
{
  starclient->unsent_messages_queue_size = queue_size;
}

h_core_bool_t h_net_starclient_star_available(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_bool_t available;
  unsigned long client_count;

  client_count = h_container_set_get_size(starclient->clients);

  if (client_count > 0) {
    available = h_core_bool_true;
  } else {
    available = h_core_bool_false;
  }

  return available;
}

void print_stats(h_net_starclient_t *starclient)
{
  assert(starclient);
  unsigned long star_arm_count;

  star_arm_count = h_container_set_get_size(starclient->clients);

  printf("[star] connected to %lu arms of the star\n", star_arm_count);
  starclient->last_star_arm_count = star_arm_count;
  starclient->need_to_print_stats = h_core_bool_false;
}

h_core_bool_t put_messsage_in_unsent_queue(h_net_starclient_t *starclient,
    h_core_message_t *message)
{
  assert(starclient);
  assert(message);
  unsigned long messages_in_queue;
  h_core_bool_t success;

  messages_in_queue = h_container_list_get_size(starclient->unsent_messages);

  if (starclient->unsent_messages_queue_size > messages_in_queue) {
    if (h_container_list_add_last(starclient->unsent_messages, message)) {
      success = h_core_bool_true;
    } else {
      success = h_core_bool_false;
    }
  } else {
    success = h_core_bool_false;
  }

  return success;
}

void re_route_unsent_messages(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_message_t *message;
  h_container_list_t *unsent_messages;

  unsent_messages = starclient->unsent_messages;

  h_container_list_iterate_start(unsent_messages);
  while ((message = h_container_list_iterate_next(unsent_messages))) {
    h_net_starclient_send_message_to_any_arm(starclient, message);
    h_container_list_iterate_remove(unsent_messages);
  }
}

void rebuild_client_list(h_net_starclient_t *starclient)
{
  assert(starclient);
  h_core_nameobject_t *nameclient;

  h_container_list_clear(starclient->client_list);
  h_container_set_iterate_start(starclient->clients);
  while ((nameclient = h_container_set_iterate_next(starclient->clients))) {
    h_container_list_add_last(starclient->client_list, nameclient);
  }
}

h_core_bool_t register_engines_with_client(h_net_starclient_t *starclient,
    h_net_client_t *client)
{
  assert(starclient);
  assert(client);
  h_core_bool_t success;
  unsigned long engine_id;
  long message_type_count_long;
  unsigned long message_type_count;

  success = h_core_bool_true;

  for (engine_id = 0; engine_id < H_NET_ENGINE_TYPE_COUNT; engine_id++) {
    message_type_count_long = *(starclient->message_type_counts + engine_id);
    if (-1 != message_type_count_long) {
      message_type_count = (unsigned long) message_type_count_long;
      if (!h_net_client_register_engine
          (client, engine_id, message_type_count)) {
        h_core_trace("h_net_client_register_engine");
        success = h_core_bool_false;
      }
    }
  }

  return success;
}

void register_message_handlers_with_client(h_net_starclient_t *starclient,
    h_net_client_t *client)
{
  assert(starclient);
  assert(client);
  message_handler_info_t *message_handler_info;

  h_container_list_iterate_start(starclient->message_handler_info);
  while ((message_handler_info
          = h_container_list_iterate_next(starclient->message_handler_info))) {
    h_net_client_register_message_handler(client,
        message_handler_info->engine_id, message_handler_info->message_type,
        message_handler_info->message_handler);
  }
}

void take_unsent_messages(h_net_starclient_t *starclient,
    h_net_client_t *client)
{
  assert(starclient);
  assert(client);
  h_container_list_t *unsent_messages;
  h_core_message_t *message;
  unsigned long discarded_message_count;

  discarded_message_count = 0;

  unsent_messages = h_net_client_take_unsent_messages(client);
  h_container_list_iterate_start(unsent_messages);
  while ((message = h_container_list_iterate_next(unsent_messages))) {
    if (!put_messsage_in_unsent_queue(starclient, message)) {
      discarded_message_count++;
      h_core_message_destroy(message);
    }
  }
  h_container_list_destroy(unsent_messages);

  if (discarded_message_count > 0) {
    printf("[star] unsent message queue is full, unable to take %lu "
        "unsent messages from client\n", discarded_message_count);
  }
}
