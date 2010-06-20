#ifndef h_core_bit_h
#define h_core_bit_h

#include "h/core/object.h"

typedef unsigned char h_core_bit_t;

int h_core_bit_compare(void *bit_object_a, void *bit_object_b);

void *h_core_bit_copy(void *bit_object);

void h_core_bit_destroy(void *bit_object);

#endif
