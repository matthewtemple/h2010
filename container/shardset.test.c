#include "h/container/shardset.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"

#define SHARD_COUNT H_CONTAINER_SHARDSET_MAX_SHARDS

int main(int argc, char *argv[])
{
  h_container_shardset_t *shardset;
  char *string;

  shardset = h_container_shardset_create(h_core_string_compare,
      h_core_string_copy, H_CORE_NO_DESTROY_FUNCTION, h_core_string_hash,
      SHARD_COUNT);
  if (!shardset) {
    h_core_trace_exit("h_container_shardset_create");
  }

  h_container_shardset_add(shardset, "apple");
  h_container_shardset_add(shardset, "bicycle");
  h_container_shardset_add(shardset, "color");

  h_container_shardset_iterate_start(shardset);
  while ((string = h_container_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
    if (0 == strcmp("bicycle", string)) {
      h_container_shardset_iterate_remove(shardset);
    }
  }

  printf("\n");

  h_container_shardset_iterate_start(shardset);
  while ((string = h_container_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
  }

  return 0;
}
