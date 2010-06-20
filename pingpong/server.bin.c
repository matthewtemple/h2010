#include "h/audit/log.h"
#include "h/container/list.h"
#include "h/core/messagey.h"
#include "h/core/tools.h"
#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/enginey.h"
#include "h/net/post.h"
#include "h/net/postey.h"
#include "h/net/server.h"
#include "h/pingpong/engine.h"
#include "h/pingpong/message.h"

#define PING_SERVER_MAX_THREADS 32

struct ping_server_context_t {
  void *server_wide_shared_database;
};
typedef struct ping_server_context_t ping_server_context_t;

static void print_use_exit();

void print_use_exit()
{
  printf("\n    use: ping_server server_port\n\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  h_net_server_t *server;
  ping_server_context_t custom_server_context;
  unsigned short server_port;
  h_net_enginey_t enginey;
  h_core_messagey_t messagey;
  h_net_postey_t postey;
  h_audit_log_t *log;

  if (argc < 2) {
    print_use_exit();
  }
  server_port = atoi(argv[1]);

  log = h_audit_log_create(stdout);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  h_net_enginey_init(&enginey, h_pingpong_engine_create, h_pingpong_engine_destroy,
      h_pingpong_engine_get_handler_for_message, h_pingpong_engine_maintain,
      h_pingpong_engine_run, h_pingpong_engine_start, h_pingpong_engine_stop);

  h_core_messagey_init(&messagey, h_core_message_destroy,
      h_core_message_get_client_socket, h_core_message_get_engine_id,
      h_core_message_get_type);

  h_net_postey_init(&postey, h_net_post_compare, h_net_post_create,
      h_net_post_create_decoy, h_net_post_destroy, h_net_post_destroy_decoy,
      h_net_post_get_last_receive_activity_time, h_net_post_get_socket,
      h_net_post_get_stats, h_net_post_receive_message,
      h_net_post_receive_messages, h_net_post_send_message,
      h_net_post_send_messages, h_net_post_is_socket_closed);

  server = h_net_server_create("ping", server_port, server_port,
      PING_SERVER_MAX_THREADS, &messagey, &postey,
      H_NET_ENGINE_NO_GET_NAME_FUNCTION, H_NET_SERVER_NO_CONFIG_SYSTEM, log);
  if (!server) {
    h_core_trace_exit("h_net_server_create");
  }

  if (!h_net_server_register_engine(server, H_NET_ENGINE_PING,
          &custom_server_context, &enginey, 2, 2, H_NET_MAINTAIN_1_MINUTE,
          H_PINGPONG_MESSAGE_TYPE_COUNT)) {
    h_core_trace_exit("h_net_server_register_engine");
  }

  if (!h_net_server_start(server)) {
    h_core_trace_exit("h_net_server_start");
  }

  h_net_server_destroy(server);
  h_audit_log_destroy(log);

  return 0;
}
