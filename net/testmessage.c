#include "h/core/engine.h"
#include "h/core/message.h"
#include "h/core/messagey.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/testengine.h"
#include "h/net/testmessage.h"

h_core_message_t *h_net_testmessage_create(int client_socket,
    unsigned long message_type)
{
  return h_core_message_create(client_socket, H_NET_ENGINE_TEST, message_type,
      H_CORE_MESSAGEY_NULL_DATA, H_CORE_MESSAGEY_ZERO_DATA_SIZE);
}
