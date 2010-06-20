#ifndef h_net_post_h
#define h_net_post_h

#include "h/container/list.h"
#include "h/core/message.h"
#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/post_stats.h"

struct h_net_post_t;
typedef struct h_net_post_t h_net_post_t;

int h_net_post_compare(void *post_object_a, void *post_object_b);

void *h_net_post_create(int socket);

void *h_net_post_create_decoy(int socket);

void h_net_post_destroy(void *post_object);

void h_net_post_destroy_decoy(void *post_object);

time_t h_net_post_get_last_receive_activity_time(void *post_object);

int h_net_post_get_socket(void *post_object);

void h_net_post_get_stats(void *post_object, h_net_post_stats_t *post_stats);

h_core_bool_t h_net_post_is_socket_closed(void *post_object);

void *h_net_post_receive_message(void *post_object);

void h_net_post_receive_messages(void *post_object);

h_core_bool_t h_net_post_send_message(void *post_object, void *message_object);

void h_net_post_send_messages(void *post_object);

h_container_list_t *h_net_post_take_unsent_messages(h_net_post_t *post);

#endif
