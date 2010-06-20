#include "h/net/postey.h"

void h_net_postey_init(h_net_postey_t *postey, h_net_postey_compare_f compare,
    h_net_postey_create_f create, h_net_postey_create_decoy_f create_decoy,
    h_net_postey_destroy_f destroy, h_net_postey_destroy_decoy_f destroy_decoy,

    h_net_postey_get_last_receive_activity_time_f
    get_last_receive_activity_time,

    h_net_postey_get_socket_f get_socket, h_net_postey_get_stats_f get_stats,
    h_net_postey_receive_message_f receive_message,
    h_net_postey_receive_messages_f receive_messages,
    h_net_postey_send_message_f send_message,
    h_net_postey_send_messages_f send_messages,
    h_net_postey_socket_closed_f socket_closed)
{
  assert(postey);

  postey->compare = compare;
  postey->create = create;
  postey->create_decoy = create_decoy;
  postey->destroy = destroy;
  postey->destroy_decoy = destroy_decoy;
  postey->get_last_receive_activity_time = get_last_receive_activity_time;
  postey->get_socket = get_socket;
  postey->get_stats = get_stats;
  postey->receive_message = receive_message;
  postey->receive_messages = receive_messages;
  postey->send_message = send_message;
  postey->send_messages = send_messages;
  postey->socket_closed = socket_closed;
}
