#ifndef h_core_shard_h
#define h_core_shard_h

#include "h/core/object.h"

struct h_core_shard_t {
  unsigned long id;
  void *object;
};
typedef struct h_core_shard_t h_core_shard_t;

void h_core_shard_init(h_core_shard_t *shard, unsigned long id, void *object);

#endif
