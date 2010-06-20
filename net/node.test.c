#include "h/config/tools.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/node.h"
#include "h/net/node_conf.h"

int main(int argc, char *argv[])
{
  h_config_disable_if_running_batch_tests(argc, argv);

  h_net_node_t *node;
  h_net_node_conf_t *node_conf;
  h_audit_log_t *log;

  node_conf = h_net_node_conf_create("h_net_node.conf");
  if (!node_conf) {
    h_core_trace_exit("h_net_node_conf_create");
  }

  log = h_audit_log_create(stdout);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  node = h_net_node_create(node_conf->node_ip, node_conf->node_port,
      node_conf->node_port, node_conf->node_max_threads,
      node_conf->peer_node_ips, node_conf->peer_node_port_min,
      node_conf->peer_node_port_max, log);
  if (!node) {
    h_core_trace_exit("h_net_node_create");
  }

  h_net_node_set_server_unresponsive_client_time_seconds(node, 32);

  if (!h_net_node_start(node)) {
    h_core_trace_exit("h_net_node_start");
  }

  h_net_node_destroy(node);
  h_net_node_conf_destroy(node_conf);
  h_audit_log_destroy(log);

  return 0;
}
