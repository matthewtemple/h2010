#ifndef h_net_testmessage_h
#define h_net_testmessage_h

#include "h/net/ping.h"

#define H_NET_TESTMESSAGE_TYPE_COUNT 3
enum h_net_testmessage_t {
  H_NET_TESTMESSAGE_UNKNOWN = 0,
  H_NET_TESTMESSAGE_PING = 1,
  H_NET_TESTMESSAGE_PONG = 2
};
typedef enum h_net_testmessage_t h_net_testmessage_t;

h_core_message_t *h_net_testmessage_create(int client_socket,
    unsigned long message_type);

#endif
