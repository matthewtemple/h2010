#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/pingpong/pong.h"

struct h_pingpong_pong_t {
  char *string;
};

h_core_bool_t h_pingpong_pong_add_to_message(void *pong_object,
    h_core_message_t *message)
{
  assert(pong_object);
  assert(message);
  h_core_bool_t success;
  h_pingpong_pong_t *pong;

  pong = pong_object;

  if (h_core_message_add_string(message, pong->string)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
    h_core_trace("h_core_message_add_string");
  }

  return success;
}

h_pingpong_pong_t *h_pingpong_pong_create(char *string)
{
  assert(string);
  h_pingpong_pong_t *pong;

  pong = malloc(sizeof *pong);
  if (pong) {
    pong->string = h_core_string_copy(string);
    if (!pong->string) {
      h_core_trace("h_core_string_copy");
      free(pong);
      pong = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return pong;
}

h_pingpong_pong_t *h_pingpong_pong_create_from_message(h_core_message_t *message)
{
  assert(message);
  char *string;
  h_pingpong_pong_t *pong;

  string = h_core_message_take_string(message);
  if (string) {
    pong = h_pingpong_pong_create(string);
    if (!pong) {
      h_core_trace("h_pingpong_pong_create");
    }
    free(string);
  } else {
    h_core_trace("h_core_message_take_string");
    pong = NULL;
  }

  return pong;
}

void h_pingpong_pong_destroy(h_pingpong_pong_t *pong)
{
  assert(pong);

  free(pong->string);
  free(pong);
}
