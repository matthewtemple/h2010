#include "h/core/message.h"
#include "h/core/messagey.h"
#include "h/external/external.h"
#include "h/pingpong/engine.h"
#include "h/pingpong/message.h"

h_core_message_t *h_pingpong_message_create(int client_socket,
    unsigned long message_type)
{
  h_core_message_t *message;
  message = h_core_message_create(client_socket, H_NET_ENGINE_PING,
      message_type, H_CORE_MESSAGEY_NULL_DATA, H_CORE_MESSAGEY_ZERO_DATA_SIZE);
  return message;
}
