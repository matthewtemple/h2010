#include "h/core/bitarray.h"
#include "h/core/long.h"
#include "h/core/tools.h"
#include "h/external/external.h"

struct h_core_bitarray_t {
  unsigned long array_size;
  h_core_bit_t *array;
};

int h_core_bitarray_compare(void *bitarray_object_a,
    void *bitarray_object_b)
{
  assert(bitarray_object_a);
  assert(bitarray_object_b);
  h_core_bitarray_t *bitarray_a;
  h_core_bitarray_t *bitarray_b;
  int compare_result;
  unsigned long each_bit;
  h_core_bit_t bit_a;
  h_core_bit_t bit_b;

  bitarray_a = bitarray_object_a;
  bitarray_b = bitarray_object_b;

  compare_result = h_core_long_compare(&bitarray_a->array_size,
      &bitarray_b->array_size);
  if (0 == compare_result) {
    for (each_bit = 0; each_bit < bitarray_a->array_size; each_bit++) {
      bit_a = *(bitarray_a->array + each_bit);
      bit_b = *(bitarray_b->array + each_bit);
      if (bit_a < bit_b) {
        compare_result = -1;
        break;
      } else if (bit_a > bit_b) {
        compare_result = 1;
        break;
      }
    }
  }

  return compare_result;
}

void *h_core_bitarray_copy(void *bitarray_object)
{
  assert(bitarray_object);
  h_core_bitarray_t *bitarray;
  h_core_bitarray_t *bitarray_copy;
  unsigned long index;

  bitarray = bitarray_object;

  bitarray_copy = h_core_bitarray_create(bitarray->array_size);
  if (bitarray_copy) {
    for (index = 0; index < bitarray->array_size; index++) {
      *(bitarray_copy->array + index) = *(bitarray->array + index);
    }
  } else {
    h_core_trace("h_core_bitarray_create");
  }

  return bitarray_copy;
}

h_core_bitarray_t *h_core_bitarray_create(unsigned long size)
{
  assert(size > 0);
  h_core_bitarray_t *bitarray;
  unsigned long array_memory_size;

  bitarray = malloc(sizeof *bitarray);
  if (bitarray) {
    array_memory_size = size * sizeof(h_core_bit_t);
    bitarray->array = malloc(array_memory_size);
    if (bitarray->array) {
      memset(bitarray->array, '\0', array_memory_size);
      bitarray->array_size = size;
    } else {
      h_core_trace("malloc");
      free(bitarray);
      bitarray = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return bitarray;
}

void h_core_bitarray_destroy(void *bitarray_object)
{
  assert(bitarray_object);
  h_core_bitarray_t *bitarray;

  bitarray = bitarray_object;

  free(bitarray->array);
  free(bitarray);

}

h_core_bit_t h_core_bitarray_get_bit_actual(h_core_bitarray_t *bitarray,
    unsigned long index)
{
  return *(bitarray->array + index);
}

void h_core_bitarray_set_all(h_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 1, bitarray->array_size * sizeof(h_core_bit_t));
}

void h_core_bitarray_set_bit_actual(h_core_bitarray_t *bitarray,
    unsigned long index, h_core_bit_t value)
{
  *(bitarray->array + index) = value;
}

unsigned long h_core_bitarray_get_size(h_core_bitarray_t *bitarray)
{
  return bitarray->array_size;
}

unsigned long h_core_bitarray_get_memory_size_bytes
(h_core_bitarray_t *bitarray)
{
  assert(bitarray);
  unsigned long size;

  size = sizeof *bitarray;
  size += bitarray->array_size * sizeof(h_core_bit_t);

  return size;
}

void h_core_bitarray_unset_all(h_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 0, bitarray->array_size * sizeof(h_core_bit_t));
}
