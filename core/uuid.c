#include "h/core/bool.h"
#include "h/core/tools.h"
#include "h/core/uuid.h"
#include "h/external/external.h"

#define NULL_UUID "a8fffbb4-675c-4896-81f0-6eccf8bd8760"

struct h_core_uuid_t {
  uuid_t uuid;
  char *string;
};

h_core_bool_t h_core_uuid_add_to_message(void *uuid_object,
    h_core_message_t *message)
{
  assert(uuid_object);
  assert(message);
  h_core_bool_t success;
  char *uuid_string;
  h_core_uuid_t *uuid;

  uuid = uuid_object;

  uuid_string = h_core_uuid_get_string(uuid);
  if (h_core_message_add_string(message, uuid_string)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
    h_core_trace("h_core_message_add_string");
  }

  return success;
}

int h_core_uuid_compare(void *uuid_object_a,
    void *uuid_object_b)
{
  assert(uuid_object_a);
  assert(uuid_object_b);
  h_core_uuid_t *uuid_a;
  h_core_uuid_t *uuid_b;

  uuid_a = uuid_object_a;
  uuid_b = uuid_object_b;

  return uuid_compare(uuid_a->uuid, uuid_b->uuid);
}

void *h_core_uuid_copy(void *uuid_object)
{
  assert(uuid_object);
  h_core_uuid_t *original;
  h_core_uuid_t *copy;

  original = uuid_object;

  copy = malloc(sizeof *copy);
  if (copy) {
    copy->string = NULL;
    uuid_copy(copy->uuid, original->uuid);
  } else {
    h_core_trace("malloc");
  }

  return copy;
}

h_core_uuid_t *h_core_uuid_create()
{
  h_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid_generate(uuid->uuid);
    uuid->string = NULL;
  } else {
    h_core_trace("malloc");
  }

  return uuid;
}

void *h_core_uuid_create_from_message(h_core_message_t *message)
{
  assert(message);
  h_core_uuid_t *uuid;
  char *uuid_string;

  uuid_string = h_core_message_take_string(message);
  if (uuid_string) {
    uuid = h_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      h_core_trace("h_core_uuid_create_from_string");
    }
    free(uuid_string);
  } else {
    h_core_trace("h_core_message_take_string");
    uuid = NULL;
  }

  return uuid;
}

h_core_uuid_t *h_core_uuid_create_from_string(const char *uuid_string)
{
  h_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid->string = NULL;
    if (0 != uuid_parse(uuid_string, uuid->uuid)) {
      free(uuid);
      uuid = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return uuid;
}

h_core_uuid_t *h_core_uuid_create_null()
{
  return h_core_uuid_create_from_string(NULL_UUID);
}

void h_core_uuid_destroy(void *uuid_object)
{
  assert(uuid_object);
  h_core_uuid_t *uuid;

  uuid = uuid_object;

  if (uuid->string) {
    if (strcmp(uuid->string, NULL_UUID) != 0) {
      free(uuid->string);
    }
  }
  free(uuid);
}

unsigned long h_core_uuid_get_memory_size_bytes(h_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long size;

  size = sizeof *uuid;
  size += sizeof(uuid_t);

  return size;
}

char *h_core_uuid_get_string(void *uuid_object)
{
  assert(uuid_object);
  h_core_uuid_t *uuid;

  uuid = uuid_object;

  if (!uuid->string) {
    uuid->string = malloc(37);
    if (uuid->string) {
      uuid_unparse(uuid->uuid, uuid->string);
    } else {
      uuid->string = NULL_UUID;
      h_core_trace("malloc");
    }
  }

  assert(uuid->string);
  return uuid->string;
}

unsigned long h_core_uuid_hash(h_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long hash;

  h_core_uuid_get_string(uuid);
  hash = h_core_hash_djb2_xor(uuid->string);

  return hash;
}

h_core_bool_t h_core_uuid_is_null(h_core_uuid_t *uuid)
{
  assert(uuid);
  h_core_bool_t is_null;
  h_core_uuid_t *null_uuid;

  is_null = h_core_bool_false;

  null_uuid = h_core_uuid_create_null();
  if (null_uuid) {
    if (0 == h_core_uuid_compare(uuid, null_uuid)) {
      is_null = h_core_bool_true;
    }
    h_core_uuid_destroy(null_uuid);
  } else {
    h_core_trace("h_core_uuid_create_null");
  }

  return is_null;
}
