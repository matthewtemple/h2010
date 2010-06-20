#include "h/container/list.h"
#include "h/core/constants.h"
#include "h/core/period.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/shell/system.h"

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
  return 0;

  h_core_shell_t *shell;
  h_core_period_t *period;
  h_container_list_t *input;
  char *command_string;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  shell = h_core_shell_create();
  if (!shell) {
    h_core_trace_exit("h_core_shell_create");
  }

  if (!h_core_shell_start(shell)) {
    h_core_trace_exit("h_core_shell_start");
  }

  period = h_core_period_create(2);
  if (!period) {
    h_core_trace_exit("h_core_period_create");
  }

  while (!h_core_stop_requested) {
    input = h_core_shell_take_input(shell);
    if (input) {
      h_container_list_iterate_start(input);
      while ((command_string = h_container_list_iterate_next(input))) {
        printf(":%s:\n", command_string);
      }
      h_container_list_destroy(input);
    }

    if (h_core_bool_false && h_core_period_once(period)) {
      h_core_shell_append_output(shell, "ok\n");
    }

    usleep(H_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  h_core_period_destroy(period);
  h_core_shell_destroy(shell);

  return 0;
}
