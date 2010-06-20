#include "h/container/list.h"
#include "h/container/set.h"
#include "h/core/content.h"
#include "h/core/messagey.h"
#include "h/core/nameobject.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/core/types.h"
#include "h/external/external.h"
#include "h/net/hypermessage.h"

struct h_net_hypermessage_t {
  int client_socket;

  h_net_hypermethod_t hypermethod;
  h_net_hyperstatus_t hyperstatus;
  char *resource_path;
  h_net_hyperversion_t hyperversion;
  h_container_set_t *hyperheaders;
  char *body;
  unsigned long body_size;

  h_core_content_t content_type;
  char *resource_name;
  h_container_set_t *pri_parameters;
};

static h_core_bool_t h_net_hypermessage_create_pri
(h_net_hypermessage_t *hypermessage);

static void h_net_hypermessage_create_rollback
(h_net_hypermessage_t *hypermessage);

/*
  TODO: simplify
*/
h_net_hypermessage_t *h_net_hypermessage_create(int client_socket,
    h_net_hypermethod_t hypermethod, h_net_hyperstatus_t hyperstatus,
    char *resource_path, h_net_hyperversion_t hyperversion,
    h_container_set_t *hyperheaders)
{
  h_net_hypermessage_t *hypermessage;
  h_core_bool_t so_far_so_good;

  hypermessage = malloc(sizeof *hypermessage);
  if (hypermessage) {
    hypermessage->client_socket = client_socket;
    hypermessage->hypermethod = hypermethod;
    hypermessage->hyperstatus = hyperstatus;
    hypermessage->hyperversion = hyperversion;
    hypermessage->body = NULL;
    hypermessage->body_size = 0;
    hypermessage->resource_name = NULL;
    hypermessage->pri_parameters = NULL;

    if (resource_path) {
      hypermessage->resource_path = strdup(resource_path);
      if (hypermessage->resource_path) {
        so_far_so_good = h_core_bool_true;
      } else {
        h_core_trace("strdup");
        so_far_so_good = h_core_bool_false;
      }
    } else {
      hypermessage->resource_path = NULL;
      so_far_so_good = h_core_bool_true;
    }

  } else {
    so_far_so_good = h_core_bool_false;
    h_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (hyperheaders) {
      hypermessage->hyperheaders = h_container_set_copy(hyperheaders);
    } else {
      hypermessage->hyperheaders
        = h_container_set_create(h_core_nameobject_compare,
            h_core_nameobject_copy, h_core_nameobject_destroy);
      if (!hypermessage->hyperheaders) {
        so_far_so_good = h_core_bool_false;
        h_core_trace("h_container_set_create");
      }
    }
  }

  if (so_far_so_good) {
    if (hypermessage->resource_path) {
      if (!h_net_hypermessage_create_pri(hypermessage)) {
        so_far_so_good = h_core_bool_false;
      }
    }
  }

  if (!so_far_so_good && hypermessage) {
    h_net_hypermessage_create_rollback(hypermessage);
    hypermessage = NULL;
  }

  assert(!hypermessage || hypermessage->hyperheaders);
  return hypermessage;
}

h_core_bool_t h_net_hypermessage_create_pri(h_net_hypermessage_t *hypermessage)
{
  assert(hypermessage->resource_path);
  h_core_bool_t success;
  char *parameters;
  char *parameter;
  char *name;
  char *value;
  char *parameter_context;
  char *nameobject_context;
  h_core_nameobject_t *nameobject;
  char *resource_name;

  nameobject_context = NULL;

  resource_name = strtok(hypermessage->resource_path, "?");
  hypermessage->resource_name = strdup(resource_name);
  parameters = strtok(NULL, "?");
  if (parameters) {
    hypermessage->pri_parameters = h_container_set_create
      (h_core_nameobject_compare, h_core_nameobject_copy,
          h_core_nameobject_destroy);
    if (hypermessage->pri_parameters) {
      success = h_core_bool_true;
      parameter = strtok_r(parameters, "&", &parameter_context);
      while (parameter) {
        name = strtok_r(parameter, "=", &nameobject_context);
        value = strtok_r(NULL, "=", &nameobject_context);
        if (name && value) {
          nameobject = h_core_nameobject_create(name, value,
              h_core_string_copy, h_core_string_destroy,
              h_core_string_get_as_string);
          if (nameobject) {
            if (!h_container_set_add
                (hypermessage->pri_parameters, nameobject)) {
              h_core_nameobject_destroy(nameobject);
            }
          } else {
            h_core_trace("h_core_nameobject_create");
            success = h_core_bool_false;
          }
        }
        parameter = strtok_r(NULL, "&", &parameter_context);
      }
    } else {
      h_core_trace("h_container_set_create");
      success = h_core_bool_false;
    }
  } else {
    hypermessage->pri_parameters = NULL;
    success = h_core_bool_true;
  }

  return success;
}

void h_net_hypermessage_create_rollback(h_net_hypermessage_t *hypermessage)
{
  assert(hypermessage);

  if (hypermessage->resource_path) {
    free(hypermessage->resource_path);
  }
  if (hypermessage->hyperheaders) {
    h_container_set_destroy(hypermessage->hyperheaders);
  }
  if (hypermessage->body) {
    free(hypermessage->body);
  }
  free(hypermessage);
}

void h_net_hypermessage_destroy(void *hypermessage_object)
{
  assert(hypermessage_object);
  h_net_hypermessage_t *hypermessage;

  hypermessage = hypermessage_object;

  if (hypermessage->body) {
    free(hypermessage->body);
  }

  if (hypermessage->resource_path) {
    free(hypermessage->resource_path);
  }

  if (hypermessage->resource_name) {
    free(hypermessage->resource_name);
  }

  if (hypermessage->pri_parameters) {
    h_container_set_destroy(hypermessage->pri_parameters);
  }

  h_container_set_destroy(hypermessage->hyperheaders);

  free(hypermessage);
}

char *h_net_hypermessage_get_body(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->body;
}

unsigned long h_net_hypermessage_get_body_size
(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->body_size;
}

int h_net_hypermessage_get_client_socket(void *hypermessage_object)
{
  assert(hypermessage_object);
  h_net_hypermessage_t *hypermessage;

  hypermessage = hypermessage_object;

  return hypermessage->client_socket;
}

h_net_engine_id_t h_net_hypermessage_get_engine_id
(void *message_object)
{
  return H_NET_ENGINE_HYPER;
}

h_core_nameobject_t *h_net_hypermessage_get_hyperheader
(h_net_hypermessage_t *hypermessage, char *header_name)
{
  assert(hypermessage);
  assert(header_name);
  h_core_nameobject_t *decoy_header;
  h_core_nameobject_t *found_header;

  decoy_header = h_core_nameobject_create_decoy(header_name);
  found_header = h_container_set_find
    (hypermessage->hyperheaders, decoy_header);

  return found_header;
}

h_container_set_t *h_net_hypermessage_get_hyperheaders
(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperheaders;
}

h_net_hypermethod_t h_net_hypermessage_get_hypermethod
(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->hypermethod;
}

h_net_hyperstatus_t h_net_hypermessage_get_hyperstatus
(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperstatus;
}

h_net_hyperversion_t h_net_hypermessage_get_hyperversion
(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperversion;
}

char *h_net_hypermessage_get_resource_name(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->resource_name;
}

char *h_net_hypermessage_get_pri_parameter(h_net_hypermessage_t *hypermessage,
    char *parameter_name)
{
  char *parameter_value;
  h_core_nameobject_t *nameobject_decoy;
  h_core_nameobject_t *nameobject;

  if (hypermessage->pri_parameters) {
    nameobject_decoy = h_core_nameobject_create_decoy(parameter_name);
    if (nameobject_decoy) {
      nameobject
        = h_container_set_find(hypermessage->pri_parameters, nameobject_decoy);
      if (nameobject) {
        parameter_value = h_core_nameobject_get_object(nameobject);
      } else {
        parameter_value = NULL;
      }
      h_core_nameobject_destroy_decoy(nameobject_decoy);
    } else {
      h_core_trace("h_core_nameobject_create_decoy");
      parameter_value = NULL;
    }
  } else {
    parameter_value = NULL;
  }

  return parameter_value;
}

double h_net_hypermessage_get_pri_parameter_as_double
(h_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  char *value_string;
  double value_double;

  value_double = 0.0;

  value_string
    = h_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    value_double = atof(value_string);
    free(value_string);
  } else {
    h_core_trace("h_net_hypermessage_get_pri_parameter");
  }

  return value_double;
}

h_core_bool_t h_net_hypermessage_get_pri_parameter_as_unsigned_long
(h_net_hypermessage_t *hypermessage, char *parameter_name,
    unsigned long *value)
{
  assert(hypermessage);
  assert(parameter_name);
  assert(value);
  char *value_string;
  h_core_bool_t success;

  value_string
    = h_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    success = h_core_bool_true;
    *value = atol(value_string);
  } else {
    success = h_core_bool_false;
    h_core_trace("h_net_hypermessage_get_pri_parameter");
  }

  return success;
}

h_core_uuid_t *h_net_hypermessage_get_pri_parameter_as_uuid
(h_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  h_core_uuid_t *uuid;
  char *uuid_string;

  uuid = NULL;

  uuid_string
    = h_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (uuid_string) {
    uuid = h_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      h_core_trace("h_core_uuid_create_from_string");
    }
  } else {
    h_core_trace("h_net_hypermessage_get_pri_parameter");
  }

  return uuid;
}

h_container_set_t *h_net_hypermessage_get_pri_parameter_as_uuid_set
(h_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  h_core_uuid_t *uuid;
  char *uuid_string;
  char *parameter_string;
  h_container_list_t *uuid_strings;
  h_container_set_t *uuid_set;

  uuid_set = h_container_set_create
    (h_core_uuid_compare, h_core_uuid_copy, h_core_uuid_destroy);
  if (uuid_set) {
    parameter_string
      = h_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
    if (parameter_string) {
      uuid_strings
        = h_container_list_create_strings_from_string(parameter_string, ",");
      if (uuid_strings) {
        h_container_list_iterate_start(uuid_strings);
        while ((uuid_string = h_container_list_iterate_next(uuid_strings))) {
          uuid = h_core_uuid_create_from_string(uuid_string);
          if (uuid) {
            if (!h_container_set_find(uuid_set, uuid)) {
              if (!h_container_set_add(uuid_set, uuid)) {
                h_core_uuid_destroy(uuid);
                h_core_trace("h_container_set_add");
              }
            } else {
              h_core_uuid_destroy(uuid);
            }
          } else {
            h_core_trace("h_core_uuid_create_from_string");
          }
        }
        h_container_list_destroy(uuid_strings);
      } else {
        h_core_trace("h_container_list_create_string_from_strings");
      }
    } else {
      h_core_trace("h_net_hypermessage_get_pri_parameter");
    }
  } else {
    h_core_trace("h_container_set_create");
  }

  return uuid_set;
}

h_container_set_t *h_net_hypermessage_get_pri_parameters
(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->pri_parameters;
}

char *h_net_hypermessage_get_resource_path(h_net_hypermessage_t *hypermessage)
{
  return hypermessage->resource_path;
}

unsigned long h_net_hypermessage_get_type(void *message_object)
{
  return H_CORE_MESSAGEY_NO_TYPE;
}

h_core_bool_t h_net_hypermessage_set_body(h_net_hypermessage_t *hypermessage,
    h_core_content_t content_type, char *body, unsigned long body_size)
{
  assert(hypermessage);
  assert(body);
  assert(body_size > 0);
  h_core_bool_t success;
  char *content_type_name;
  char *body_size_string;

  content_type_name = h_core_content_get_name(content_type);
  success = h_core_bool_true;

  if (!h_net_hypermessage_set_hyperheader
      (hypermessage, "Content-Type", content_type_name)) {
    success = h_core_bool_false;
  }

  if (asprintf(&body_size_string, "%lu", body_size) != -1) {
    if (!h_net_hypermessage_set_hyperheader
        (hypermessage, "Content-Length", body_size_string)) {
      success = h_core_bool_false;
    }
    free(body_size_string);
  } else {
    h_core_trace("asprintf");
  }

  if (success) {
    hypermessage->body = malloc(body_size);
    if (hypermessage->body) {
      memcpy(hypermessage->body, body, body_size);
      hypermessage->body_size = body_size;
    } else {
      h_core_trace("malloc");
      success = h_core_bool_false;
    }
  }

  return success;
}

h_core_bool_t h_net_hypermessage_set_hyperheader
(h_net_hypermessage_t *hypermessage, char *header_name, char *header_value)
{
  assert(hypermessage);
  assert(header_name);
  assert(header_value);
  h_core_bool_t success;
  h_core_nameobject_t *nameobject;

  nameobject = h_core_nameobject_create(header_name, header_value,
      h_core_string_copy, h_core_string_destroy, h_core_string_get_as_string);
  if (nameobject) {
    if (h_container_set_add(hypermessage->hyperheaders, nameobject)) {
      success = h_core_bool_true;
    } else {
      success = h_core_bool_false;
    }
  } else {
    success = h_core_bool_false;
  }

  return success;
}
