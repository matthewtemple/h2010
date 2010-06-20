#ifndef h_pingpong_pong_h
#define h_pingpong_pong_h

#include "h/core/bool.h"
#include "h/core/message.h"

struct h_pingpong_pong_t;
typedef struct h_pingpong_pong_t h_pingpong_pong_t;

h_core_bool_t h_pingpong_pong_add_to_message(void *pong_object,
    h_core_message_t *message);

h_pingpong_pong_t *h_pingpong_pong_create(char *string);

h_pingpong_pong_t *h_pingpong_pong_create_from_message(h_core_message_t *message);

void h_pingpong_pong_destroy(h_pingpong_pong_t *pong);

#endif
