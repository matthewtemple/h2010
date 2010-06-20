#include "h/core/object.h"
#include "h/core/tools.h"
#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/engine_thread.h"
#include "h/net/server.h"
#include "h/pingpong/engine.h"
#include "h/pingpong/message.h"
#include "h/pingpong/ping.h"
#include "h/pingpong/pong.h"

struct h_pingpong_engine_t {
  void *things_the_engine_needs_to_share;
  h_net_server_t *h_net_server;
  void *custom_server_object;
};

static h_net_message_status_t handle_ping(void *engine_object,
    void *message_object);

h_net_message_status_t handle_ping(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  h_pingpong_ping_t *ping;
  h_pingpong_pong_t *pong;
  h_core_message_t *pong_message;
  h_pingpong_engine_t *engine;
  h_net_server_t *h_net_server;
  h_net_message_status_t message_status;
  int client_socket;
  h_core_message_t *message;
  char *ping_string;

  message = message_object;
  engine = engine_object;
  h_net_server = engine->h_net_server;
  client_socket = h_core_message_get_client_socket(message);

  ping = h_pingpong_ping_create_from_message(message);
  if (ping) {
    ping_string = h_pingpong_ping_get_string(ping);
    if (ping_string) {
      pong = h_pingpong_pong_create(ping_string);
      if (pong) {
        pong_message = h_pingpong_message_create
          (client_socket, H_PINGPONG_MESSAGE_PONG);
        if (pong_message) {
          if (h_pingpong_pong_add_to_message(pong, pong_message)) {
            if (h_net_server_send_message(h_net_server, pong_message)) {
              message_status = H_NET_MESSAGE_STATUS_HANDLED;
            } else {
              h_core_trace("h_net_server_send_message() failed");
              message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
              h_core_message_destroy(pong_message);
            }
          } else {
            h_core_trace("h_pingpong_pong_add_to_message");
            message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
            h_core_message_destroy(pong_message);
          }
        } else {
          h_core_trace("h_pingpong_message_create");
          message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
        }
        h_pingpong_pong_destroy(pong);
      } else {
        h_core_trace("h_pingpong_pong_create");
        message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
      }
      h_pingpong_ping_destroy(ping);
    } else {
      h_core_trace("h_pingpong_ping_get_string");
      message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
    }
  } else {
    h_core_trace("h_pingpong_ping_create");
    message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
  }

  return message_status;
}

void *h_pingpong_engine_create(h_net_server_t *h_net_server,
    void *custom_server_object)
{
  assert(h_net_server);
  assert(custom_server_object);
  h_pingpong_engine_t *engine;

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->h_net_server = h_net_server;
    engine->custom_server_object = custom_server_object;
  }

  h_net_server_register_message_handler(h_net_server, H_NET_ENGINE_PING,
      H_PINGPONG_MESSAGE_PING, handle_ping);

  return engine;
}

void h_pingpong_engine_destroy(void *engine_object)
{
  free(engine_object);
}

h_net_server_handle_message_f h_pingpong_engine_get_handler_for_message
(void *engine_object, void *message_object)
{
  assert(engine_object);
  assert(message_object);
  h_pingpong_engine_t *engine;
  h_net_server_handle_message_f handler;
  h_core_message_t *message;

  engine = engine_object;
  message = message_object;
  handler
    = h_net_server_get_handler_for_message(engine->h_net_server, message);

  return handler;
}

void h_pingpong_engine_maintain(void *engine_object)
{
  assert(engine_object);
  h_pingpong_engine_t *engine;
  h_net_server_t *h_net_server;

  engine = engine_object;
  h_net_server = engine->h_net_server;

  h_net_server_print_stats(h_net_server);
}

void h_pingpong_engine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_pingpong_engine_t *engine;
  h_net_server_t *h_net_server;
  unsigned char thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  h_net_server = engine->h_net_server;
  thread_index = engine_thread->thread_index;

  h_net_server_process_messages(h_net_server, H_NET_ENGINE_PING, thread_index);
}

void h_pingpong_engine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_pingpong_engine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("[ping] ping engine starting\n");
}

void h_pingpong_engine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_pingpong_engine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("[ping] ping engine stopping\n");
}
