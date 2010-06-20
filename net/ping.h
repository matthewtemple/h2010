#ifndef h_net_ping_h
#define h_net_ping_h

#include "h/core/message.h"

struct h_net_ping_t;
typedef struct h_net_ping_t h_net_ping_t;

h_core_bool_t h_net_ping_add_to_message(void *ping_object,
    h_core_message_t *message);

h_net_ping_t *h_net_ping_create(const char *string);

h_net_ping_t *h_net_ping_create_from_message(h_core_message_t *message);

void h_net_ping_destroy(h_net_ping_t *ping);

char *h_net_ping_get_string(h_net_ping_t *ping);

#endif
