#include "h/core/messagey.h"
#include "h/core/types.h"
#include "h/external/external.h"

void h_core_messagey_init(h_core_messagey_t *messagey,
    h_core_messagey_destroy_f destroy,
    h_core_messagey_get_client_socket_f get_client_socket,
    h_core_messagey_get_engine_id_f get_engine_id,
    h_core_messagey_get_type_f get_type)
{
  assert(messagey);

  messagey->destroy = destroy;
  messagey->get_client_socket = get_client_socket;
  messagey->get_engine_id = get_engine_id;
  messagey->get_type = get_type;
}
