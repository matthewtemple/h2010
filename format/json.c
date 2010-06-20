#include "h/core/tools.h"
#include "h/format/json.h"

struct h_format_json_t {
};

h_format_json_t *h_format_json_create()
{
  h_format_json_t *json;

  json = malloc(sizeof *json);
  if (json) {

  } else {
    h_core_trace("malloc");
  }

  return json;
}

void h_format_json_destroy(h_format_json_t *json)
{
  assert(json);
  free(json);
}
