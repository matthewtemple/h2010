#ifndef h_net_postey_h
#define h_net_postey_h

#include "h/core/object.h"
#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/post_stats.h"

typedef int (*h_net_postey_compare_f)(void *a_object, void *b_object);

typedef void *(*h_net_postey_create_f)(int socket);

typedef void *(*h_net_postey_create_decoy_f)(int socket);

typedef void (*h_net_postey_destroy_f)(void *post_object);

typedef void (*h_net_postey_destroy_decoy_f)(void *post_object);

typedef time_t (*h_net_postey_get_last_receive_activity_time_f)
(void *post_object);

typedef int (*h_net_postey_get_socket_f)(void *post_object);

typedef void (*h_net_postey_get_stats_f)(void *post_object,
    h_net_post_stats_t *post_stats);

typedef void *(*h_net_postey_receive_message_f)(void *post_object);

typedef void (*h_net_postey_receive_messages_f)(void *post_object);

typedef h_core_bool_t (*h_net_postey_send_message_f)(void *post_object,
    void *message_object);

typedef void (*h_net_postey_send_messages_f)(void *post_object);

typedef h_core_bool_t (*h_net_postey_socket_closed_f)(void *post_object);

struct h_net_postey_t {
  h_net_postey_compare_f compare;
  h_net_postey_create_f create;
  h_net_postey_create_decoy_f create_decoy;
  h_net_postey_destroy_f destroy;
  h_net_postey_destroy_decoy_f destroy_decoy;

  h_net_postey_get_last_receive_activity_time_f
  get_last_receive_activity_time;

  h_net_postey_get_socket_f get_socket;
  h_net_postey_get_stats_f get_stats;
  h_net_postey_receive_message_f receive_message;
  h_net_postey_receive_messages_f receive_messages;
  h_net_postey_send_message_f send_message;
  h_net_postey_send_messages_f send_messages;
  h_net_postey_socket_closed_f socket_closed;
};
typedef struct h_net_postey_t h_net_postey_t;

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
    h_net_postey_socket_closed_f socket_closed);

#endif
