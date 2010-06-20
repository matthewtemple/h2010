#include "h/audit/log.h"
#include "h/config/tools.h"
#include "h/container/list.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/net/enginey.h"
#include "h/net/hypermessage.h"
#include "h/net/hyperpost.h"
#include "h/net/post.h"
#include "h/net/server.h"
#include "h/net/testengine.h"
#include "h/net/testhyperengine.h"
#include "h/net/testmessage.h"

#define TEST_SERVER_MAX_THREADS 32
#define TEST_HYPERSERVER_MAX_THREADS 32

struct h_core_test_server_context_t {
  void *server_wide_shared_database;
};
typedef struct h_core_test_server_context_t h_core_test_server_context_t;

int main(int argc, char *argv[])
{
  h_config_disable_if_running_batch_tests(argc, argv);

  h_core_test_server_context_t custom_server_context;

  h_net_server_t *server;
  unsigned short port;
  h_net_enginey_t enginey;
  h_core_messagey_t messagey;
  h_net_postey_t postey;

  h_net_server_t *hyperserver;
  unsigned short hyperport;
  h_net_enginey_t hyperenginey;
  h_core_messagey_t hypermessagey;
  h_net_postey_t hyperpostey;

  h_audit_log_t *log;

  log = h_audit_log_create(stdout);
  if (!log) {
    h_core_trace_exit("h_audit_log_create");
  }

  port = 20000;

  h_net_enginey_init(&enginey, h_net_testengine_create,
      h_net_testengine_destroy, h_net_testengine_get_handler_for_message,
      h_net_testengine_maintain, h_net_testengine_run,
      h_net_testengine_start, h_net_testengine_stop);

  h_core_messagey_init(&messagey, h_core_message_destroy,
      h_core_message_get_client_socket, h_core_message_get_engine_id,
      h_core_message_get_type);

  h_net_postey_init(&postey, h_net_post_compare, h_net_post_create,
      h_net_post_create_decoy, h_net_post_destroy, h_net_post_destroy_decoy,
      h_net_post_get_last_receive_activity_time, h_net_post_get_socket,
      h_net_post_get_stats, h_net_post_receive_message,
      h_net_post_receive_messages, h_net_post_send_message,
      h_net_post_send_messages, h_net_post_is_socket_closed);

  server = h_net_server_create("standard", port, port, TEST_SERVER_MAX_THREADS,
      &messagey, &postey, H_NET_ENGINE_NO_GET_NAME_FUNCTION,
      H_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(server);

  if (!h_net_server_register_engine(server, H_NET_ENGINE_TEST,
          &custom_server_context, &enginey, 2, 2, H_NET_MAINTAIN_CONSTANTLY,
          H_NET_TESTMESSAGE_TYPE_COUNT)) {
    h_core_trace_exit("h_net_server_register_engine");
  }

  hyperport = 50000;

  h_net_enginey_init(&hyperenginey, h_net_testhyperengine_create,
      h_net_testhyperengine_destroy,
      h_net_testhyperengine_get_handler_for_message,
      h_net_testhyperengine_maintain, h_net_testhyperengine_run,
      h_net_testhyperengine_start, h_net_testhyperengine_stop);

  h_core_messagey_init(&hypermessagey, h_net_hypermessage_destroy,
      h_net_hypermessage_get_client_socket, h_net_hypermessage_get_engine_id,
      h_net_hypermessage_get_type);

  h_net_postey_init(&hyperpostey, h_net_hyperpost_compare,
      h_net_hyperpost_create, h_net_hyperpost_create_decoy,
      h_net_hyperpost_destroy, h_net_hyperpost_destroy_decoy,
      h_net_hyperpost_get_last_receive_activity_time,
      h_net_hyperpost_get_socket, h_net_hyperpost_get_stats,
      h_net_hyperpost_receive_message, h_net_hyperpost_receive_messages,
      h_net_hyperpost_send_message, h_net_hyperpost_send_messages,
      h_net_hyperpost_is_socket_closed);

  hyperserver = h_net_server_create("http", hyperport, hyperport,
      TEST_HYPERSERVER_MAX_THREADS, &hypermessagey, &hyperpostey,
      H_NET_ENGINE_NO_GET_NAME_FUNCTION, H_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(hyperserver);

  if (!h_net_server_start(server)) {
    h_core_trace_exit("h_net_server_start");
  }

  if (!h_net_server_start(hyperserver)) {
    h_core_trace_exit("h_net_server_start");
  }

  h_net_server_destroy(server);
  h_net_server_destroy(hyperserver);
  h_audit_log_destroy(log);

  return 0;
}
