#include "h/core/shard.h"
#include "h/core/types.h"
#include "h/external/external.h"

void h_core_shard_init(h_core_shard_t *shard, unsigned long id,
    void *object)
{
  assert(shard);
  shard->id = id;
  shard->object = object;
}
