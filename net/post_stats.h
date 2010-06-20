#ifndef h_net_post_stats_h
#define h_net_post_stats_h

struct h_net_post_stats_t {
  unsigned long messages_sent;
  unsigned long messages_received;
  unsigned long messages_in_inbox;
  unsigned long messages_in_outbox;
  unsigned long most_messages_ever_in_inbox;
  unsigned long most_messages_ever_in_outbox;
  unsigned long send_message_failures;
  unsigned long receive_message_failures;
  unsigned long incoming_message_size_pseudo_average;
  unsigned long messages_not_sent_due_to_socket_send_failures;
};
typedef struct h_net_post_stats_t h_net_post_stats_t;

void h_net_post_stats_init(h_net_post_stats_t *post_stats);

#endif
