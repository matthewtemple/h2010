#ifndef h_net_hyperpost_h
#define h_net_hyperpost_h

#include "h/core/object.h"
#include "h/core/types.h"
#include "h/net/post_stats.h"

struct h_net_hyperpost_t;
typedef struct h_net_hyperpost_t h_net_hyperpost_t;

int h_net_hyperpost_compare(void *hyperpost_object_a,
    void *hyperpost_object_b);

void *h_net_hyperpost_create(int socket);

void *h_net_hyperpost_create_decoy(int socket);

void h_net_hyperpost_destroy(void *hyperpost_object);

void h_net_hyperpost_destroy_decoy(void *hyperpost_object);

time_t h_net_hyperpost_get_last_receive_activity_time(void *hyperpost_object);

int h_net_hyperpost_get_socket(void *hyperpost_object);

void h_net_hyperpost_get_stats(void *hyperpost_object,
    h_net_post_stats_t *post_stats);

h_core_bool_t h_net_hyperpost_is_socket_closed(void *hyperpost_object);

void *h_net_hyperpost_receive_message(void *hyperpost_object);

void h_net_hyperpost_receive_messages(void *hyperpost_object);

h_core_bool_t h_net_hyperpost_send_message(void *hyperpost_object,
    void *hypermessage_object);

void h_net_hyperpost_send_messages(void *hyperpost_object);

#endif
