#include "h/container/set.h"
#include "h/core/tools.h"
#include "h/net/exchange.h"

struct h_net_exchange_t {
  h_container_set_t *posts;
  h_net_postey_t *postey;
};

h_net_exchange_t *h_net_exchange_create(h_net_postey_t *postey)
{
  assert(postey);
  h_net_exchange_t *exchange;
  h_core_bool_t success;

  exchange = malloc(sizeof *exchange);
  if (exchange) {
    success = h_core_bool_true;
    exchange->postey = postey;
    exchange->posts = h_container_set_create(postey->compare,
        H_CORE_NO_COPY_FUNCTION, H_CORE_NO_DESTROY_FUNCTION);
    if (!exchange->posts) {
      h_core_trace("h_container_set_create");
      success = h_core_bool_false;
    }
  } else {
    h_core_trace("malloc");
    success = h_core_bool_false;
  }

  if (!success && exchange) {
    h_container_set_destroy(exchange->posts);
    free(exchange);
    exchange = NULL;
  }

  return exchange;
}

void h_net_exchange_destroy(h_net_exchange_t *exchange)
{
  h_container_set_destroy(exchange->posts);
  free(exchange);
}

unsigned long h_net_exchange_get_post_count(h_net_exchange_t *exchange)
{
  return h_container_set_get_size(exchange->posts);
}

h_core_bool_t h_net_exchange_register_post(h_net_exchange_t *exchange,
    void *post_object)
{
  return h_container_set_add(exchange->posts, post_object);
}

void h_net_exchange_send_and_receive_messages(h_net_exchange_t *exchange)
{
  fd_set read_sockets;
  fd_set write_sockets;
  int max_socket;
  void *post_object;
  int socket;
  struct timeval select_timeout;

  FD_ZERO(&read_sockets);
  FD_ZERO(&write_sockets);
  max_socket = 0;
  select_timeout.tv_sec = 0;
  select_timeout.tv_usec = 0;

  h_container_set_iterate_start(exchange->posts);
  while ((post_object = h_container_set_iterate_next(exchange->posts))) {
    socket = exchange->postey->get_socket(post_object);
    FD_SET(socket, &read_sockets);
    FD_SET(socket, &write_sockets);
    if (socket > max_socket) {
      max_socket = socket;
    }
  }

  if (select(max_socket + 1, &read_sockets, &write_sockets, NULL,
          &select_timeout) > 0) {
    h_container_set_iterate_start(exchange->posts);
    while ((post_object = h_container_set_iterate_next(exchange->posts))) {
      socket = exchange->postey->get_socket(post_object);
      if (FD_ISSET(socket, &read_sockets)) {
        exchange->postey->receive_messages(post_object);
      }
      if (FD_ISSET(socket, &write_sockets)) {
        exchange->postey->send_messages(post_object);
      }
    }
  }
}

h_core_bool_t h_net_exchange_unregister_post(h_net_exchange_t *exchange,
    int socket)
{
  void *decoy_post_object;
  h_core_bool_t success;

  decoy_post_object = exchange->postey->create_decoy(socket);
  if (decoy_post_object) {
    success = h_core_bool_true;
    if (!h_container_set_remove(exchange->posts, decoy_post_object)) {
      h_core_do_nothing();
      h_core_trace("h_container_set_remove");
      success = h_core_bool_false;
    }
    exchange->postey->destroy_decoy(decoy_post_object);
  } else {
    h_core_trace("postey->create_decoy");
    success = h_core_bool_false;
  }

  return success;
}
