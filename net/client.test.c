#include "h/config/tools.h"
#include "h/core/constants.h"
#include "h/core/message.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/client.h"
#include "h/net/ping.h"
#include "h/net/testengine.h"
#include "h/net/testmessage.h"

#define H_NET_CLIENT_TEST_SLEEP_MICROSECONDS H_CORE_STANDARD_SLEEP_MICROSECONDS

struct test_client_context_t {
  h_core_bool_t pong_received;
};
typedef struct test_client_context_t test_client_context_t;

static void handle_pong(void *custom_client_context,
    h_core_message_t *message);

void handle_pong(void *custom_client_context, h_core_message_t *message)
{
  assert(custom_client_context);
  assert(message);
  test_client_context_t *client_context;
  char *pong_string;

  client_context = custom_client_context;

  client_context->pong_received = h_core_bool_true;
  pong_string = h_core_message_take_string(message);
  printf("%s\n", pong_string);

  free(pong_string);
  assert(client_context->pong_received);
}

int main(int argc, char *argv[])
{
  h_config_disable_if_running_batch_tests(argc, argv);

  h_net_client_t *client;
  h_core_message_t *ping_message;
  test_client_context_t test_client_context;
  h_net_ping_t *ping;
  h_audit_log_t *log;

  test_client_context.pong_received = h_core_bool_false;

  log = h_audit_log_create(stdout);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  client = h_net_client_create("127.0.0.1", 20000, 20000,
      H_NET_ENGINE_NO_GET_NAME_FUNCTION, &test_client_context, log);
  if (!client) {
    h_core_trace_exit("h_net_client_create()");
  }
  h_net_client_register_engine(client, H_NET_ENGINE_TEST,
      H_NET_TESTMESSAGE_TYPE_COUNT);
  h_net_client_register_message_handler(client, H_NET_ENGINE_TEST,
      H_NET_TESTMESSAGE_PONG, handle_pong);

  ping = h_net_ping_create("penta");
  if (!ping) {
    h_core_trace_exit("h_net_ping_create");
  }

  ping_message = h_net_testmessage_create(H_CORE_MESSAGEY_NO_CLIENT_SOCKET,
      H_NET_TESTMESSAGE_PING);
  if (!ping_message) {
    h_core_trace_exit("h_net_testmessage_create");
  }

  if (!h_net_ping_add_to_message(ping, ping_message)) {
    h_core_trace_exit("h_net_ping_add_to_message");
  }

  h_net_ping_destroy(ping);

  if (h_net_client_send_message(client, ping_message)) {
    printf("client sent ping message\n");
  } else {
    h_core_message_destroy(ping_message);
    h_core_trace_exit("h_net_client_send_message");
  }

  do {
    h_net_client_process_messages(client);
    usleep(H_NET_CLIENT_TEST_SLEEP_MICROSECONDS);
  } while (!test_client_context.pong_received);

  h_net_client_destroy(client);
  h_audit_log_destroy(log);

  return 0;
}
