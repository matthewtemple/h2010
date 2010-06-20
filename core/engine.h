#ifndef h_net_engine_h
#define h_net_engine_h

#define H_NET_ENGINE_NO_GET_NAME_FUNCTION NULL

#define H_NET_ENGINE_TYPE_COUNT 5
enum h_net_engine_id_t {
  H_NET_ENGINE_UNKNOWN = 0,
  H_NET_ENGINE_TEST = 1,
  H_NET_ENGINE_PING = 2,
  H_NET_ENGINE_SHELL = 3,
  H_NET_ENGINE_HYPER = 4,
  H_NET_ENGINE_MAX = 31,
};
typedef enum h_net_engine_id_t h_net_engine_id_t;

typedef char *(*h_net_engine_get_name_f)(unsigned long engine_id);

char *h_net_engine_get_name(unsigned long engine_id,
    h_net_engine_get_name_f get_engine_name);

#endif
