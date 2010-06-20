#ifndef h_net_exchange_h
#define h_net_exchange_h

#include "h/net/postey.h"

struct h_net_exchange_t;
typedef struct h_net_exchange_t h_net_exchange_t;

h_net_exchange_t *h_net_exchange_create(h_net_postey_t *postey);

void h_net_exchange_destroy(h_net_exchange_t *exchange);

unsigned long h_net_exchange_get_post_count(h_net_exchange_t *exchange);

h_core_bool_t h_net_exchange_register_post(h_net_exchange_t *exchange,
    void *post_object);

void h_net_exchange_send_and_receive_messages(h_net_exchange_t *exchange);

h_core_bool_t h_net_exchange_unregister_post(h_net_exchange_t *exchange,
    int socket);

#endif
