#ifndef h_pingpong_message_h
#define h_pingpong_message_h

#define H_PINGPONG_MESSAGE_TYPE_COUNT 3
enum h_pingpong_message_t {
  H_PINGPONG_MESSAGE_UNKNOWN = 0,
  H_PINGPONG_MESSAGE_PING = 1,
  H_PINGPONG_MESSAGE_PONG = 2
};
typedef enum h_pingpong_message_t h_pingpong_message_t;

h_core_message_t *h_pingpong_message_create(int client_socket,
    unsigned long message_type);

#endif
