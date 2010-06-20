#ifndef h_net_webrequest_h
#define h_net_webrequest_h

struct h_net_webrequest_t;
typedef struct h_net_webrequest_t h_net_webrequest_t;

h_net_webrequest_t *h_net_webrequest_create();

void h_net_webrequest_destroy(h_net_webrequest_t *webrequest);

#endif
