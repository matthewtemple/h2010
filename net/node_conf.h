#ifndef h_net_node_conf_h
#define h_net_node_conf_h

#include "h/config/file.h"

struct h_net_node_conf_t {
  h_config_file_t *conf;

  char *node_ip;
  unsigned short node_port;
  unsigned short node_max_threads;

  h_container_list_t *peer_node_ips;
  unsigned short peer_node_port_min;
  unsigned short peer_node_port_max;
};
typedef struct h_net_node_conf_t h_net_node_conf_t;

h_net_node_conf_t *h_net_node_conf_create(char *conf_filename);

void h_net_node_conf_destroy(h_net_node_conf_t *node_conf);

#endif
