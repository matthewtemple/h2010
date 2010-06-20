#ifndef h_pingpong_engine_h
#define h_pingpong_engine_h

#include "h/core/engine.h"
#include "h/net/server.h"

struct h_pingpong_engine_t;
typedef struct h_pingpong_engine_t h_pingpong_engine_t;

void *h_pingpong_engine_create(h_net_server_t *h_net_server,
    void *custom_server_object);

void h_pingpong_engine_destroy(void *engine_object);

h_net_server_handle_message_f h_pingpong_engine_get_handler_for_message
(void *engine_object, void *message_object);

void h_pingpong_engine_maintain(void *engine_object);

void h_pingpong_engine_run(void *engine_thread_object);

void h_pingpong_engine_start(void *engine_thread_object);

void h_pingpong_engine_stop(void *engine_thread_object);

#endif
