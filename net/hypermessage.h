#ifndef h_net_hypermessage_h
#define h_net_hypermessage_h

#include "h/container/set.h"
#include "h/core/content.h"
#include "h/core/engine.h"
#include "h/core/nameobject.h"
#include "h/core/object.h"
#include "h/core/uuid.h"
#include "h/net/hyperversion.h"

#define H_NET_HYPERMESSAGE_NO_HEADERS NULL
#define H_NET_HYPERMESSAGE_NO_RESOURCE_PATH NULL

enum h_net_hypermethod_t {
  H_NET_HYPERMETHOD_UNKNOWN,
  H_NET_HYPERMETHOD_GET,
  H_NET_HYPERMETHOD_HEAD,
  H_NET_HYPERMETHOD_POST,
};
typedef enum h_net_hypermethod_t h_net_hypermethod_t;

enum h_net_hyperstatus_t {
  H_NET_HYPERSTATUS_UNKNOWN,
  H_NET_HYPERSTATUS_OK,
  H_NET_HYPERSTATUS_NOT_FOUND,
};
typedef enum h_net_hyperstatus_t h_net_hyperstatus_t;

struct h_net_hypermessage_t;
typedef struct h_net_hypermessage_t h_net_hypermessage_t;

h_net_hypermessage_t *h_net_hypermessage_create(int client_socket,
    h_net_hypermethod_t hypermethod, h_net_hyperstatus_t hyperstatus,
    char *resource_path, h_net_hyperversion_t hyperversion,
    h_container_set_t *hyperheaders);

void h_net_hypermessage_destroy(void *hypermessage_object);

char *h_net_hypermessage_get_body(h_net_hypermessage_t *hypermessage);

unsigned long h_net_hypermessage_get_body_size
(h_net_hypermessage_t *hypermessage);

int h_net_hypermessage_get_client_socket(void *hypermessage_object);

h_net_engine_id_t h_net_hypermessage_get_engine_id(void *message_object);

h_core_nameobject_t *h_net_hypermessage_get_hyperheader
(h_net_hypermessage_t *hypermessage, char *header_name);

h_container_set_t *h_net_hypermessage_get_hyperheaders
(h_net_hypermessage_t *hypermessage);

h_net_hypermethod_t h_net_hypermessage_get_hypermethod
(h_net_hypermessage_t *hypermessage);

h_net_hyperstatus_t h_net_hypermessage_get_hyperstatus
(h_net_hypermessage_t *hypermessage);

h_net_hyperversion_t h_net_hypermessage_get_hyperversion
(h_net_hypermessage_t *hypermessage);

char *h_net_hypermessage_get_resource_name(h_net_hypermessage_t *hypermessage);

char *h_net_hypermessage_get_pri_parameter(h_net_hypermessage_t *hypermessage,
    char *parameter_name);

double h_net_hypermessage_get_pri_parameter_as_double
(h_net_hypermessage_t *hypermessage, char *parameter_name);

h_core_bool_t h_net_hypermessage_get_pri_parameter_as_unsigned_long
(h_net_hypermessage_t *hypermessage, char *parameter_name,
    unsigned long *value);

h_core_uuid_t *h_net_hypermessage_get_pri_parameter_as_uuid
(h_net_hypermessage_t *hypermessage, char *parameter_name);

h_container_set_t *h_net_hypermessage_get_pri_parameter_as_uuid_set
(h_net_hypermessage_t *hypermessage, char *parameter_name);

h_container_set_t *h_net_hypermessage_get_pri_parameters
(h_net_hypermessage_t *hypermessage);

char *h_net_hypermessage_get_resource_path(h_net_hypermessage_t *hypermessage);

unsigned long h_net_hypermessage_get_type(void *message_object);

h_core_bool_t h_net_hypermessage_set_body(h_net_hypermessage_t *hypermessage,
    h_core_content_t content_type, char *body, unsigned long body_size);

h_core_bool_t h_net_hypermessage_set_hyperheader
(h_net_hypermessage_t *hypermessage, char *header_name, char *header_value);

#endif
