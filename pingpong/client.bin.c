#include "h/core/constants.h"
#include "h/core/message.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/client.h"
#include "h/pingpong/engine.h"
#include "h/pingpong/message.h"
#include "h/pingpong/ping.h"

#define PING_CLIENT_SLEEP_MICROSECONDS H_CORE_STANDARD_SLEEP_MICROSECONDS
#define PING_CLIENT_SEND_MESSAGE_COUNT 8

struct ping_client_context_t {
  h_core_bool_t pong_received;
  unsigned short pongs_received;
};
typedef struct ping_client_context_t ping_client_context_t;

static void handle_pong(void *custom_client_context,
    h_core_message_t *message);
static void print_usage_exit();

void handle_pong(void *custom_client_context, h_core_message_t *message)
{
  assert(custom_client_context);
  assert(message);
  ping_client_context_t *client_context;
  char *pong_string;

  client_context = custom_client_context;

  client_context->pong_received = h_core_bool_true;
  client_context->pongs_received++;
  pong_string = h_core_message_take_string(message);
  printf("[ping] server sent back \"%s\"\n", pong_string);

  free(pong_string);
}

void print_usage_exit()
{
  printf("\n    usage: ping_client serverIp serverPort pingMessage\n\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  h_net_client_t *client;
  h_core_message_t *ping_message;
  ping_client_context_t ping_client_context;
  h_pingpong_ping_t *ping;
  h_audit_log_t *log;

  unsigned short server_port;
  char *server_ip;
  char *ping_string;

  unsigned short message_count;

  signal(SIGPIPE, SIG_IGN);

  if (argc < 4) {
    print_usage_exit();
  }
  server_ip = argv[1];
  server_port = atoi(argv[2]);
  ping_string = argv[3];

  ping_client_context.pong_received = h_core_bool_false;
  ping_client_context.pongs_received = 0;

  log = h_audit_log_create(stdout);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  client = h_net_client_create(server_ip, server_port, server_port,
      H_NET_ENGINE_NO_GET_NAME_FUNCTION, &ping_client_context, log);
  if (!client) {
    h_core_trace_exit("h_net_client_create");
  }

  if (!h_net_client_register_engine(client, H_NET_ENGINE_PING,
          H_PINGPONG_MESSAGE_TYPE_COUNT)) {
    h_core_trace_exit("h_net_client_register_engine");
  }

  h_net_client_register_message_handler(client, H_NET_ENGINE_PING,
      H_PINGPONG_MESSAGE_PONG, handle_pong);

  for (message_count = 0; message_count < PING_CLIENT_SEND_MESSAGE_COUNT;
       message_count++) {

    ping = h_pingpong_ping_create(ping_string);
    if (ping) {
      ping_message = h_pingpong_message_create
        (H_CORE_MESSAGEY_NO_CLIENT_SOCKET, H_PINGPONG_MESSAGE_PING);
      if (ping_message) {
        if (h_pingpong_ping_add_to_message(ping, ping_message)) {
          if (!h_net_client_send_message(client, ping_message)) {
            h_core_trace_exit("h_net_client_send_message() failed");
          }
        } else {
          h_core_trace("h_pingpong_ping_add_to_message");
          h_core_message_destroy(ping_message);
        }
      } else {
        h_core_trace("h_pingpong_message_create");
      }
      h_pingpong_ping_destroy(ping);
    } else {
      h_core_trace_exit("h_pingpong_ping_create");
    }
  }

  do {
    h_net_client_process_messages(client);
    usleep(PING_CLIENT_SLEEP_MICROSECONDS);
  } while (ping_client_context.pongs_received
      < PING_CLIENT_SEND_MESSAGE_COUNT);

  h_net_client_destroy(client);
  h_audit_log_destroy(log);

  return 0;
}
