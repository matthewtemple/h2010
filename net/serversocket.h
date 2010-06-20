#ifndef h_net_serversocket_h
#define h_net_serversocket_h

#include "h/external/external.h"

int h_net_serversocket_accept(int serversocket,
    struct sockaddr_in *client_address, socklen_t *client_address_size);

int h_net_serversocket_create(unsigned short port);

void h_net_serversocket_destroy(int serversocket);

#endif
