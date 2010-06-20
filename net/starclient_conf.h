#ifndef h_net_starclient_conf_h
#define h_net_starclient_conf_h

#include "h/config/file.h"

struct h_net_starclient_conf_t {
  h_config_file_t *conf;

  h_container_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_port;
};
typedef struct h_net_starclient_conf_t h_net_starclient_conf_t;

h_net_starclient_conf_t *h_net_starclient_conf_create(char *conf_filename);

void h_net_starclient_conf_destroy(h_net_starclient_conf_t *starclient_conf);

#endif
