#ifndef h_net_types_h
#define h_net_types_h

#include "h/core/object.h"

enum h_net_maintain_t {
  H_NET_MAINTAIN_CONSTANTLY = 0,
  H_NET_MAINTAIN_2_SECONDS = 2,
  H_NET_MAINTAIN_4_SECONDS = 4,
  H_NET_MAINTAIN_1_MINUTE = 60,
  H_NET_MAINTAIN_1_HOUR = 60 * 60,
};
typedef enum h_net_maintain_t h_net_maintain_t;

enum h_net_message_status_t {
  H_NET_MESSAGE_STATUS_HANDLED,
  H_NET_MESSAGE_STATUS_CANT_HANDLE,
  H_NET_MESSAGE_STATUS_CANT_HANDLE_NOW,
};
typedef enum h_net_message_status_t h_net_message_status_t;

typedef h_net_message_status_t (*h_net_server_handle_message_f)
(void *engine_object, void *message_object);

struct h_net_server_t;
typedef struct h_net_server_t h_net_server_t;

#endif
