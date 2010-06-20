#ifndef h_net_clientsocket_h
#define h_net_clientsocket_h

int h_net_clientsocket_create(char *server_ip_address,
    unsigned short server_port);

void h_net_clientsocket_destroy(int clientsocket);

#endif
