#ifndef h_net_enginey_h
#define h_net_enginey_h

#include "h/core/object.h"
#include "h/net/types.h"

typedef void *(*h_net_enginey_create_f)
  (h_net_server_t *server, void *custom_server_context_object);

typedef void (*h_net_enginey_destroy_f)(void *engine_object);

typedef h_net_server_handle_message_f(*h_net_enginey_get_handler_for_message_f)
(void *engine_object, void *message_object);

typedef void (*h_net_enginey_maintain_f)(void *engine_thread_context_object);

typedef void (*h_net_enginey_run_f)(void *engine_thread_context_object);

typedef void (*h_net_enginey_start_f)(void *engine_thread_context_object);

typedef void (*h_net_enginey_stop_f)(void *engine_thread_context_object);

struct h_net_enginey_t {
  h_net_enginey_create_f create;
  h_net_enginey_destroy_f destroy;
  h_net_enginey_get_handler_for_message_f get_handler_for_message;
  h_net_enginey_maintain_f maintain;
  h_net_enginey_run_f run;
  h_net_enginey_start_f start;
  h_net_enginey_stop_f stop;
};
typedef struct h_net_enginey_t h_net_enginey_t;

void h_net_enginey_init(h_net_enginey_t *enginey,
    h_net_enginey_create_f create, h_net_enginey_destroy_f destroy,
    h_net_enginey_get_handler_for_message_f get_handler_for_message,
    h_net_enginey_maintain_f maintain, h_net_enginey_run_f run,
    h_net_enginey_start_f start, h_net_enginey_stop_f stop);

#endif
