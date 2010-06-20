#ifndef h_net_hyperversion_h
#define h_net_hyperversion_h

enum h_net_hyperversion_t {
  H_NET_HYPERVERSION_UNKNOWN,
  H_NET_HYPERVERSION_1_0,
  H_NET_HYPERVERSION_1_1,
};
typedef enum h_net_hyperversion_t h_net_hyperversion_t;

const char *h_net_hyperversion_get_name(h_net_hyperversion_t hyperversion);

#endif
