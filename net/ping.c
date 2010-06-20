#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/ping.h"

struct h_net_ping_t {
  char *string;
};

h_core_bool_t h_net_ping_add_to_message(void *ping_object,
    h_core_message_t *message)
{
  assert(ping_object);
  assert(message);
  h_core_bool_t success;
  h_net_ping_t *ping;

  ping = ping_object;

  success = h_core_message_add_string(message, ping->string);

  return success;
}

h_net_ping_t *h_net_ping_create(const char *string)
{
  assert(string);
  h_net_ping_t *ping;

  ping = malloc(sizeof *ping);
  if (ping) {
    ping->string = strdup(string);
    if (!ping->string) {
      h_core_trace("strdup");
      free(ping);
      ping = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return ping;
}

h_net_ping_t *h_net_ping_create_from_message(h_core_message_t *message)
{
  assert(message);
  h_net_ping_t *ping;
  char *string;

  string = h_core_message_take_string(message);
  if (string) {
    ping = h_net_ping_create(string);
    if (!ping) {
      h_core_trace("h_net_ping_create");
    }
    free(string);
  } else {
    h_core_trace("h_core_message_take_string");
    ping = NULL;
  }

  return ping;
}

void h_net_ping_destroy(h_net_ping_t *ping)
{
  assert(ping);

  free(ping->string);
  free(ping);
}

char *h_net_ping_get_string(h_net_ping_t *ping)
{
  return ping->string;
}
