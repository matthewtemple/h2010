#ifndef h_net_testengine_h
#define h_net_testengine_h

#include "h/core/engine.h"
#include "h/net/server.h"

struct h_net_testengine_t;
typedef struct h_net_testengine_t h_net_testengine_t;

void *h_net_testengine_create(h_net_server_t *h_net_server,
    void *custom_server_object);

void h_net_testengine_destroy(void *engine_object);

h_net_server_handle_message_f h_net_testengine_get_handler_for_message
(void *engine_object, void *message_object);

void h_net_testengine_maintain(void *engine_object);

void h_net_testengine_run(void *engine_thread_object);

void h_net_testengine_start(void *engine_thread_object);

void h_net_testengine_stop(void *engine_thread_object);

#endif
