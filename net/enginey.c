#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/enginey.h"

void h_net_enginey_init(h_net_enginey_t *enginey,
    h_net_enginey_create_f create, h_net_enginey_destroy_f destroy,
    h_net_enginey_get_handler_for_message_f get_handler_for_message,
    h_net_enginey_maintain_f maintain, h_net_enginey_run_f run,
    h_net_enginey_start_f start, h_net_enginey_stop_f stop)
{
  assert(enginey);

  enginey->create = create;
  enginey->destroy = destroy;
  enginey->get_handler_for_message = get_handler_for_message;
  enginey->maintain = maintain;
  enginey->run = run;
  enginey->start = start;
  enginey->stop = stop;
}
