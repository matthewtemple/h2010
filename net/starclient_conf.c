#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/starclient_conf.h"

#define DEFAULT_SERVER_EXCLUDE_IP "127.0.0.1"
#define DEFAULT_NODE_SERVER_EXCLUDE_PORT 8000
#define DEFAULT_STAR_ARM_PORT_MIN 9000
#define DEFAULT_STAR_ARM_PORT_MAX 10000

static h_core_bool_t h_net_starclient_conf_create_node_server_exclude_ip
(h_net_starclient_conf_t *starclient_conf);

static h_core_bool_t h_net_starclient_conf_create_node_server_exclude_port
(h_net_starclient_conf_t *starclient_conf);

static h_core_bool_t h_net_starclient_conf_create_star_arm_port_range
(h_net_starclient_conf_t *starclient_conf);

static h_core_bool_t h_net_starclient_conf_create_star_arm_ips
(h_net_starclient_conf_t *starclient_conf);

h_net_starclient_conf_t *h_net_starclient_conf_create(char *conf_filename)
{
  assert(conf_filename);
  h_net_starclient_conf_t *starclient_conf;
  h_core_bool_t so_far_so_good;

  starclient_conf = malloc(sizeof *starclient_conf);
  if (starclient_conf) {
    starclient_conf->conf = h_config_file_create(conf_filename);
    if (starclient_conf->conf) {
      so_far_so_good = h_core_bool_true;
    } else {
      so_far_so_good = h_core_bool_false;
    }
  } else {
    h_core_trace("malloc");
    so_far_so_good = h_core_bool_false;
  }

  if (so_far_so_good) {
    so_far_so_good = h_net_starclient_conf_create_node_server_exclude_ip
      (starclient_conf);
  }

  if (so_far_so_good) {
    so_far_so_good
      = h_net_starclient_conf_create_node_server_exclude_port(starclient_conf);
  }

  if (so_far_so_good) {
    so_far_so_good
      = h_net_starclient_conf_create_star_arm_ips(starclient_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = h_net_starclient_conf_create_star_arm_port_range
      (starclient_conf);
  }

  if (!so_far_so_good && starclient_conf) {
    if (starclient_conf->conf) {
      h_config_file_destroy(starclient_conf->conf);
    }
    free(starclient_conf);
    starclient_conf = NULL;
  }

  return starclient_conf;
}

h_core_bool_t h_net_starclient_conf_create_node_server_exclude_ip
(h_net_starclient_conf_t *starclient_conf)
{
  assert(starclient_conf);
  h_core_bool_t success;

  if (h_config_file_find_as_string(starclient_conf->conf,
          "node_server_exclude_ip", &starclient_conf->node_server_exclude_ip,
          DEFAULT_SERVER_EXCLUDE_IP)) {
    success = h_core_bool_true;
  } else {
    printf("starclient conf doesn't specify node_server_exclude_ip\n");
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_net_starclient_conf_create_node_server_exclude_port
(h_net_starclient_conf_t *starclient_conf)
{
  assert(starclient_conf);
  h_core_bool_t success;

  if (h_config_file_find_as_unsigned_short(starclient_conf->conf,
          "node_server_exclude_port",
          &starclient_conf->node_server_exclude_port,
          DEFAULT_NODE_SERVER_EXCLUDE_PORT)) {
    success = h_core_bool_true;
  } else {
    printf("starclient conf doesn't specify node_server_exclude_port\n");
    starclient_conf->node_server_exclude_ip = 0;
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_net_starclient_conf_create_star_arm_ips
(h_net_starclient_conf_t *starclient_conf)
{
  assert(starclient_conf);
  h_core_bool_t success;

  if (h_config_file_find_list_as_strings(starclient_conf->conf,
          "star_arm_ips[list]", &starclient_conf->star_arm_ips)) {
    success = h_core_bool_true;
  } else {
    printf("starclient conf doesn't specify star_arm_ip[list]\n");
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_net_starclient_conf_create_star_arm_port_range
(h_net_starclient_conf_t *starclient_conf)
{
  assert(starclient_conf);
  h_core_bool_t success;
  h_core_bool_t found_star_arm_port_min;
  h_core_bool_t found_star_arm_port_max;

  found_star_arm_port_min = h_config_file_find_as_unsigned_short
    (starclient_conf->conf, "star_arm_port_min",
        &starclient_conf->star_arm_port_min, DEFAULT_STAR_ARM_PORT_MIN);
  found_star_arm_port_max = h_config_file_find_as_unsigned_short
    (starclient_conf->conf, "star_arm_port_max",
        &starclient_conf->star_arm_port_max, DEFAULT_STAR_ARM_PORT_MAX);

  if (found_star_arm_port_min && found_star_arm_port_max) {
    success = h_core_bool_true;
  } else {
    printf("starclient conf doesn't specify both star_arm_port_min and "
        "star_arm_port_max\n");
    success = h_core_bool_false;
  }

  return success;
}

void h_net_starclient_conf_destroy(h_net_starclient_conf_t *starclient_conf)
{
  assert(starclient_conf);
  h_config_file_destroy(starclient_conf->conf);
  free(starclient_conf);
}
