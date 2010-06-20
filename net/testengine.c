#include "h/core/tools.h"
#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/engine_thread.h"
#include "h/net/pong.h"
#include "h/net/server.h"
#include "h/net/testengine.h"
#include "h/net/testmessage.h"

struct h_net_testengine_t {
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
  h_net_ping_t *ping;
  h_core_message_t *pong_message;
  h_net_testengine_t *engine;
  h_net_server_t *h_net_server;
  h_net_message_status_t message_status;
  int client_socket;
  h_core_message_t *message;
  h_net_pong_t *pong;
  char *ping_string;

  message = message_object;
  engine = engine_object;
  h_net_server = engine->h_net_server;
  client_socket = h_core_message_get_client_socket(message);

  ping = h_net_ping_create_from_message(message);
  if (ping) {
    ping_string = h_net_ping_get_string(ping);
    if (ping_string) {
      pong = h_net_pong_create(ping_string);
      if (pong) {
        pong_message
          = h_net_testmessage_create(client_socket, H_NET_TESTMESSAGE_PONG);
        if (pong_message) {
          if (h_net_pong_add_to_message(pong, pong_message)) {
            if (h_net_server_send_message(h_net_server, pong_message)) {
              message_status = H_NET_MESSAGE_STATUS_HANDLED;
            } else {
              message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
              h_core_message_destroy(pong_message);
            }
          } else {
            message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
            h_core_trace("h_net_pong_add_to_message");
            h_core_message_destroy(pong_message);
          }
        } else {
          message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
          h_core_trace("h_net_testmessage_create");
        }
        h_net_pong_destroy(pong);
      } else {
        message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
        h_core_trace("h_net_pong_create");
      }
    } else {
      message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
      h_core_trace("h_net_ping_get_string");
    }
    h_net_ping_destroy(ping);
  } else {
    message_status = H_NET_MESSAGE_STATUS_CANT_HANDLE;
    h_core_trace("h_net_ping_create_from_message");
  }

  return message_status;
}

void *h_net_testengine_create(h_net_server_t *h_net_server,
    void *custom_server_object)
{
  assert(h_net_server);
  assert(custom_server_object);
  h_net_testengine_t *engine;

  printf("test engine creating context\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->h_net_server = h_net_server;
    engine->custom_server_object = custom_server_object;
  }

  h_net_server_register_message_handler(h_net_server, H_NET_ENGINE_TEST,
      H_NET_TESTMESSAGE_PING, handle_ping);
  return engine;
}

void h_net_testengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("test engine destroying\n");
  free(engine_object);
}

h_net_server_handle_message_f h_net_testengine_get_handler_for_message
(void *engine_object, void *message_object)
{
  assert(engine_object);
  assert(message_object);
  h_net_testengine_t *engine;
  h_net_server_handle_message_f handler;
  h_core_message_t *message;

  engine = engine_object;
  message = message_object;
  handler = h_net_server_get_handler_for_message(engine->h_net_server, message);

  return handler;
}

void h_net_testengine_maintain(void *engine_object)
{
  assert(engine_object);
  h_net_testengine_t *engine;

  engine = engine_object;
}

void h_net_testengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_net_testengine_t *engine;
  h_net_server_t *server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  server = engine->h_net_server;
  thread_index = engine_thread->thread_index;

  h_net_server_process_messages(server, H_NET_ENGINE_TEST, thread_index);
}

void h_net_testengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine starting\n");
}

void h_net_testengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine stopping\n");
}
