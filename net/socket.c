#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/socket.h"

ssize_t h_net_socket_receive(int socket, void *data, size_t data_size)
{
  return recv(socket, data, data_size, 0);
}

ssize_t h_net_socket_send(int socket, void *data, size_t data_size)
{
  return send(socket, data, data_size, 0);
}
