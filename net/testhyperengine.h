#ifndef h_net_testhyperengine_h
#define h_net_testhyperengine_h

#include "h/core/engine.h"

struct h_net_testhyperengine_t;
typedef struct h_net_testhyperengine_t h_net_testhyperengine_t;

void *h_net_testhyperengine_create(h_net_server_t *h_net_server,
    void *custom_server_object);

h_net_server_handle_message_f h_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object);

void h_net_testhyperengine_destroy(void *engine_object);

void h_net_testhyperengine_maintain(void *engine_object);

void h_net_testhyperengine_run(void *engine_thread_object);

void h_net_testhyperengine_start(void *engine_thread_object);

void h_net_testhyperengine_stop(void *engine_thread_object);

#endif
