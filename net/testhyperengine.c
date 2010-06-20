#include "h/core/content.h"
#include "h/core/types.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/engine_thread.h"
#include "h/net/hypermessage.h"
#include "h/net/server.h"
#include "h/net/testhyperengine.h"
#include "h/net/testmessage.h"

struct h_net_testhyperengine_t {
  void *things_the_engine_needs_to_share;
  h_net_server_t *h_net_server;
  void *custom_server_object;
};

static h_net_message_status_t handle_about(void *engine_object,
    void *message_object);

static h_net_message_status_t handle_not_found(void *engine_object,
    void *message_object);

h_net_message_status_t handle_about(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  h_net_testhyperengine_t *engine;
  h_net_hypermessage_t *message;
  h_net_hypermessage_t *response_message;
  int client_socket;
  h_net_server_t *h_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  h_net_server = engine->h_net_server;
  message = message_object;
  client_socket = h_core_message_get_client_socket(message);
  body = "about this site";
  body_size = strlen(body);

  response_message = h_net_hypermessage_create(client_socket,
      H_NET_HYPERMETHOD_UNKNOWN, H_NET_HYPERSTATUS_OK,
      H_NET_HYPERMESSAGE_NO_RESOURCE_PATH, H_NET_HYPERVERSION_1_0,
      H_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    h_net_hypermessage_set_body(response_message,
        H_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    h_core_trace("h_net_hypermessage_create");
  }

  if (!h_net_server_send_message(h_net_server, response_message)) {
    h_core_trace("h_net_server_send_message");
  }

  return H_NET_MESSAGE_STATUS_HANDLED;
}

h_net_message_status_t handle_not_found(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  h_net_testhyperengine_t *engine;
  h_net_hypermessage_t *message;
  h_net_hypermessage_t *response_message;
  int client_socket;
  h_net_server_t *h_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  h_net_server = engine->h_net_server;
  message = message_object;
  client_socket = h_core_message_get_client_socket(message);
  body = "testhyperengine cannot handle this request";
  body_size = strlen(body);

  response_message = h_net_hypermessage_create(client_socket,
      H_NET_HYPERMETHOD_UNKNOWN, H_NET_HYPERSTATUS_NOT_FOUND,
      H_NET_HYPERMESSAGE_NO_RESOURCE_PATH, H_NET_HYPERVERSION_1_0,
      H_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    h_net_hypermessage_set_body(response_message,
        H_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    h_core_trace("h_net_hypermessage_create");
  }

  if (!h_net_server_send_message(h_net_server, response_message)) {
    h_core_trace("h_net_server_send_message");
  }

  return H_NET_MESSAGE_STATUS_HANDLED;
}

void *h_net_testhyperengine_create(h_net_server_t *h_net_server,
    void *custom_server_object)
{
  assert(h_net_server);
  assert(custom_server_object);
  h_net_testhyperengine_t *engine;

  printf("testhyperengine creating\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->h_net_server = h_net_server;
    engine->custom_server_object = custom_server_object;
  }

  return engine;
}

void h_net_testhyperengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("testhyperengine destroying\n");
  free(engine_object);
}

h_net_server_handle_message_f h_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object)
{
  assert(engine_object);
  assert(hypermessage_object);
  h_net_testhyperengine_t *engine;
  h_net_server_handle_message_f handler;
  h_net_hypermessage_t *hypermessage;
  char *resource_name;

  engine = engine_object;
  hypermessage = hypermessage_object;

  resource_name = h_net_hypermessage_get_resource_name(hypermessage);

  if (0 == strcmp("/", resource_name)) {
    handler = handle_about;

  } else if (0 == strcmp("/about", resource_name)) {
    handler = handle_about;

  } else {
    handler = handle_not_found;

  }

  return handler;
}

void h_net_testhyperengine_maintain(void *engine_object)
{
  assert(engine_object);
  h_net_testhyperengine_t *engine;

  engine = engine_object;
}

void h_net_testhyperengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_net_testhyperengine_t *engine;
  h_net_server_t *h_net_server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  h_net_server = engine->h_net_server;
  thread_index = engine_thread->thread_index;

  h_net_server_process_messages
    (h_net_server, H_NET_ENGINE_HYPER, thread_index);
}

void h_net_testhyperengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine starting\n");
}

void h_net_testhyperengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  h_net_engine_thread_t *engine_thread;
  h_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine stopping\n");
}
