#ifndef h_core_message_h
#define h_core_message_h

#include "h/core/engine.h"
#include "h/core/object.h"
#include "h/core/types.h"

enum h_core_message_encoding_t {
  H_CORE_MESSAGE_ENCODING_UNKNOWN = 0,
  H_CORE_MESSAGE_ENCODING_PLAIN = 1,
};
typedef enum h_core_message_encoding_t h_core_message_encoding_t;

typedef h_core_bool_t h_core_message_add_to_message_f(void *object,
    h_core_message_t *message);

typedef void *h_core_message_create_from_message_f(h_core_message_t *message);

h_core_bool_t h_core_message_add_bool(h_core_message_t *message, void *object);

h_core_bool_t h_core_message_add_double(h_core_message_t *message,
    void *object);

h_core_bool_t h_core_message_add_long(h_core_message_t *message, void *object);

h_core_bool_t h_core_message_add_short(h_core_message_t *message,
    void *object);

h_core_bool_t h_core_message_add_string(h_core_message_t *message,
    void *object);

void *h_core_message_copy(void *message_object);

h_core_message_t *h_core_message_create(int client_socket,
    h_net_engine_id_t engine_id, unsigned long message_type, char *data,
    unsigned long data_size);

void h_core_message_destroy(void *message_object);

int h_core_message_get_client_socket(void *message_object);

char *h_core_message_get_data(h_core_message_t *message);

unsigned long h_core_message_get_data_get_size(h_core_message_t *message);

h_core_message_encoding_t h_core_message_get_encoding
(h_core_message_t *message);

h_net_engine_id_t h_core_message_get_engine_id(void *message_object);

unsigned long h_core_message_get_type(void *message_object);

void *h_core_message_take_bool(h_core_message_t *message);

h_core_bool_t h_core_message_take_bool_value(h_core_message_t *message);

void *h_core_message_take_double(h_core_message_t *message);

double h_core_message_take_double_value(h_core_message_t *message);

void *h_core_message_take_long(h_core_message_t *message);

long h_core_message_take_long_value(h_core_message_t *message);

void *h_core_message_take_short(h_core_message_t *message);

short h_core_message_take_short_value(h_core_message_t *message);

void *h_core_message_take_string(h_core_message_t *message);

#endif
