#include "h/net/hyperversion.h"

const char *h_net_hyperversion_get_name(h_net_hyperversion_t hyperversion)
{
  const char *name;

  switch (hyperversion) {

    case H_NET_HYPERVERSION_1_0:
      name = "HTTP/1.0";
      break;

    case H_NET_HYPERVERSION_1_1:
      name = "HTTP/1.1";
      break;

    case H_NET_HYPERVERSION_UNKNOWN:
    default:
      name = "";
      break;
  }

  return name;
}
