#include "h/config/file.h"
#include "h/core/tools.h"
#include "h/external/external.h"

#define DEFAULT_PORT_MIN 8000
#define DEFAULT_PORT_MAX 9000

int main(int argc, char *argv[])
{
  h_config_file_t *conf;
  unsigned short port_min;
  unsigned short port_max;
  h_container_list_t *node_ip_list;
  char *node_ip;

  conf = h_config_file_create("config/file_test.conf");
  if (h_core_bool_true) {

    if (!h_config_file_find_as_unsigned_short
        (conf, "port_min", &port_min, DEFAULT_PORT_MIN)) {
      h_core_trace_exit("h_config_file_find_as_unsigned_short");
    }

    if (!h_config_file_find_as_unsigned_short
        (conf, "port_max", &port_max, DEFAULT_PORT_MAX)) {
      h_core_trace_exit("h_config_file_find_as_unsigned_short");
    }

    if (!h_config_file_find_list_as_strings
        (conf, "node_ips[list]", &node_ip_list)) {
      h_core_trace_exit("h_config_file_find_list");
    }

    printf("port_min:%i\n", port_min);
    printf("port_max:%i\n", port_max);
    if (node_ip_list) {
      h_container_list_iterate_start(node_ip_list);
      while ((node_ip = h_container_list_iterate_next(node_ip_list))) {
        printf("node_ip:%s\n", node_ip);
      }
    }
  }
  h_config_file_destroy(conf);

  return 0;
}
