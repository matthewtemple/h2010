#ifndef h_core_messagey_h
#define h_core_messagey_h

#include "h/core/engine.h"
#include "h/core/object.h"

#define H_CORE_MESSAGEY_NO_CLIENT_SOCKET -1
#define H_CORE_MESSAGEY_NO_TYPE 0
#define H_CORE_MESSAGEY_NULL_DATA NULL
#define H_CORE_MESSAGEY_ZERO_DATA_SIZE 0

typedef void (*h_core_messagey_destroy_f)(void *message_object);

typedef int (*h_core_messagey_get_client_socket_f)(void *message_object);

typedef h_net_engine_id_t (*h_core_messagey_get_engine_id_f)
(void *message_object);

typedef unsigned long (*h_core_messagey_get_type_f)(void *message_object);

struct h_core_messagey_t {
  h_core_messagey_destroy_f destroy;
  h_core_messagey_get_client_socket_f get_client_socket;
  h_core_messagey_get_engine_id_f get_engine_id;
  h_core_messagey_get_type_f get_type;
};
typedef struct h_core_messagey_t h_core_messagey_t;

void h_core_messagey_init(h_core_messagey_t *messagey,
    h_core_messagey_destroy_f destroy,
    h_core_messagey_get_client_socket_f get_client_socket,
    h_core_messagey_get_engine_id_f get_engine_id,
    h_core_messagey_get_type_f get_type);

#endif
