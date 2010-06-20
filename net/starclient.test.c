#include "h/config/tools.h"
#include "h/core/constants.h"
#include "h/core/period.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/starclient.h"
#include "h/net/starclient_conf.h"

static void handle_signal(int signal);

void handle_signal(int signal)
{
  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      h_core_stop_requested = h_core_bool_true;
      break;
  }
}

int main(int argc, char *argv[])
{
  h_config_disable_if_running_batch_tests(argc, argv);

  h_net_starclient_t *starclient;
  h_net_starclient_conf_t *starclient_conf;
  h_core_period_t *period;
  void *custom_client_context;
  h_audit_log_t *log;

  custom_client_context = NULL;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  log = h_audit_log_create(stdout);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  period = h_core_period_create(8);
  if (!period) {
    h_core_trace_exit("h_core_period_create");
  }

  starclient_conf = h_net_starclient_conf_create("h_net_starclient_test.conf");
  if (!starclient_conf) {
    h_core_trace_exit("h_net_starclient_conf_create");
  }

  starclient = h_net_starclient_create(starclient_conf->star_arm_ips,
      starclient_conf->star_arm_port_min,
      starclient_conf->star_arm_port_max,
      starclient_conf->node_server_exclude_ip,
      starclient_conf->node_server_exclude_port,
      starclient_conf->node_server_exclude_port,
      custom_client_context, log);
  if (starclient) {
    if (h_net_starclient_connect(starclient)) {
      printf("starclient connected to star\n");
      while (!h_core_stop_requested) {
        h_net_starclient_process_messages(starclient);

        /*
          TODO: move this to the starclient
        */
        if (!h_net_starclient_star_available(starclient)) {
          if (h_core_period_once(period)) {
            printf("star is unavailable\n");
          }
        }

        usleep(H_CORE_STANDARD_SLEEP_MICROSECONDS);
      }
      printf("starclient shutting down gracefully\n");
    } else {
      printf("starclient could not connect to star\n");
    }
  } else {
    h_core_trace("h_net_starclient_create");
  }

  h_net_starclient_destroy(starclient);
  h_net_starclient_conf_destroy(starclient_conf);
  h_core_period_destroy(period);
  h_audit_log_destroy(log);

  return 0;
}
