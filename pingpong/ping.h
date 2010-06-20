#ifndef h_pingpong_ping_h
#define h_pingpong_ping_h

#include "h/core/bool.h"
#include "h/core/message.h"

struct h_pingpong_ping_t;
typedef struct h_pingpong_ping_t h_pingpong_ping_t;

h_core_bool_t h_pingpong_ping_add_to_message(void *ping_object,
    h_core_message_t *message);

h_pingpong_ping_t *h_pingpong_ping_create(char *string);

h_pingpong_ping_t *h_pingpong_ping_create_from_message(h_core_message_t *message);

void h_pingpong_ping_destroy(h_pingpong_ping_t *ping);

char *h_pingpong_ping_get_string(h_pingpong_ping_t *ping);

#endif
