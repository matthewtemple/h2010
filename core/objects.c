#include "h/core/long.h"
#include "h/core/objects.h"
#include "h/core/pair.h"
#include "h/core/string.h"
#include "h/core/unsigned_long.h"
#include "h/core/void.h"
#include "h/external/external.h"

void h_core_objects_free(h_core_objects_t *objects) {}

void h_core_objects_init(h_core_objects_t *objects)
{
  assert(objects);

  h_core_long_init_objectey(&objects->long_objectey);
  h_core_pair_init_objectey(&objects->pair_objectey);
  h_core_string_init_objectey(&objects->string_objectey);
  h_core_unsigned_long_init_objectey(&objects->unsigned_long_objectey);
  h_core_void_init_objectey(&objects->void_objectey);
}
