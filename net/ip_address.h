#ifndef h_net_ip_address_h
#define h_net_ip_address_h

struct h_net_ip_address_t;
typedef struct h_net_ip_address_t h_net_ip_address_t;

h_net_ip_address_t *h_net_ip_address_create_from_string(char *ip_address);

void h_net_ip_address_destroy();

#endif
