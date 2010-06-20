#ifndef h_core_bitarray_h
#define h_core_bitarray_h

#include "h/core/bit.h"
#include "h/core/bool.h"
#include "h/core/message.h"
#include "h/core/object.h"
#include "h/core/string.h"

#define H_CORE_BITARRAY_BITS_IN_CHAR 8
#define H_CORE_BITARRAY_BITS_IN_LONG 32
#define H_CORE_BITARRAY_BITS_IN_SHORT 16
#define H_CORE_BITARRAY_BITS_IN_UNSIGNED_CHAR 8
#define H_CORE_BITARRAY_BITS_IN_UNSIGNED_LONG 32
#define H_CORE_BITARRAY_BITS_IN_UNSIGNED_SHORT 16

struct h_core_bitarray_t;
typedef struct h_core_bitarray_t h_core_bitarray_t;

h_core_bool_t h_core_bitarray_add_to_message(void *bitarray_object,
    h_core_message_t *message);

char *h_core_bitarray_as_binary_string(void *bitarray_object);

int h_core_bitarray_compare(void *bitarray_object_a, void *bitarray_object_b);

void *h_core_bitarray_copy(void *bitarray_object);

h_core_bitarray_t *h_core_bitarray_create(unsigned long size);

h_core_bitarray_t *h_core_bitarray_create_from_char(char value);

h_core_bitarray_t *h_core_bitarray_create_from_char_bits(char value,
    unsigned short bits);

h_core_bitarray_t *h_core_bitarray_create_from_double(double value);

h_core_bitarray_t *h_core_bitarray_create_from_double_bits(double value,
    unsigned short integral_bits, unsigned short fractional_bits);

h_core_bitarray_t *h_core_bitarray_create_from_long(long value);

h_core_bitarray_t *h_core_bitarray_create_from_long_bits(long value,
    unsigned short bits);

h_core_bitarray_t *h_core_bitarray_create_from_message
(h_core_message_t *message);

h_core_bitarray_t *h_core_bitarray_create_from_short(short value);

h_core_bitarray_t *h_core_bitarray_create_from_short_bits(short value,
    unsigned short bits);

h_core_bitarray_t *h_core_bitarray_create_from_string(char *string,
    unsigned long string_length);

h_core_bitarray_t *h_core_bitarray_create_from_string_bits(char *string,
    unsigned long string_length, unsigned short bits);

h_core_bitarray_t *h_core_bitarray_create_from_unsigned_char
(unsigned char value);

h_core_bitarray_t *h_core_bitarray_create_from_unsigned_char_bits
(unsigned char value, unsigned short bits);

h_core_bitarray_t *h_core_bitarray_create_from_unsigned_long
(unsigned long value);

h_core_bitarray_t *h_core_bitarray_create_from_unsigned_long_bits
(unsigned long value, unsigned short bits);

h_core_bitarray_t *h_core_bitarray_create_from_unsigned_short
(unsigned short value);

h_core_bitarray_t *h_core_bitarray_create_from_unsigned_short_bits
(unsigned short value, unsigned short bits);

h_core_bitarray_t *h_core_bitarray_create_random(unsigned long size);

void h_core_bitarray_destroy(void *bitarray_object);

char *h_core_bitarray_get_as_string(void *bitarray_object);

unsigned long h_core_bitarray_get_actual_index(h_core_bitarray_t *bitarray,
    unsigned long virtual_index);

h_core_bit_t h_core_bitarray_get_bit(h_core_bitarray_t *bitarray,
    unsigned long virtual_index);

h_core_bool_t h_core_bitarray_get_bool(h_core_bitarray_t *bitarray,
    unsigned long index);

char h_core_bitarray_get_char(h_core_bitarray_t *bitarray,
    unsigned long index);

char h_core_bitarray_get_char_bits(h_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

double h_core_bitarray_get_double(h_core_bitarray_t *bitarray,
    unsigned long index);

double h_core_bitarray_get_double_bits(h_core_bitarray_t *bitarray,
    unsigned long index, unsigned short integral_bits,
    unsigned short fractional_bits);

/*
  TODO: deprecate in favor of h_core_bitarray_get_double()
*/
double h_core_bitarray_get_double_from_bits(h_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index);

long h_core_bitarray_get_long(h_core_bitarray_t *bitarray,
    unsigned long index);

long h_core_bitarray_get_long_bits(h_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long h_core_bitarray_get_memory_size_bytes
(h_core_bitarray_t *bitarray);

short h_core_bitarray_get_short(h_core_bitarray_t *bitarray,
    unsigned long index);

short h_core_bitarray_get_short_bits(h_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long h_core_bitarray_get_size(h_core_bitarray_t *bitarray);

char *h_core_bitarray_get_string(h_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned char h_core_bitarray_get_unsigned_char(h_core_bitarray_t *bitarray,
    unsigned long index);

unsigned char h_core_bitarray_get_unsigned_char_bits
(h_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

unsigned long h_core_bitarray_get_unsigned_long(h_core_bitarray_t *bitarray,
    unsigned long index);

unsigned long h_core_bitarray_get_unsigned_long_bits
(h_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

/*
  TODO: deprecate in favor of h_core_bitarray_get_unsigned_long()
*/
unsigned long h_core_bitarray_get_unsigned_long_from_bits
(h_core_bitarray_t *bitarray, unsigned long start_index,
    unsigned long end_index);

unsigned short h_core_bitarray_get_unsigned_short(h_core_bitarray_t *bitarray,
    unsigned long index);

unsigned short h_core_bitarray_get_unsigned_short_bits
(h_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

h_core_bool_t h_core_bitarray_increment(h_core_bitarray_t *bitarray);

void h_core_bitarray_print(h_core_bitarray_t *bitarray);

void h_core_bitarray_randomize(h_core_bitarray_t *bitarray);

void h_core_bitarray_set_all(h_core_bitarray_t *bitarray);

void h_core_bitarray_set_bit(h_core_bitarray_t *bitarray, unsigned long index,
    h_core_bit_t value);

void h_core_bitarray_set_bits_from_bitarray(h_core_bitarray_t *destination,
    unsigned long destination_index, h_core_bitarray_t *source,
    unsigned long source_index, unsigned long length);

void h_core_bitarray_flip_bit(h_core_bitarray_t *bitarray,
    unsigned long index);

void h_core_bitarray_set_bit(h_core_bitarray_t *bitarray,
    unsigned long virtual_index, h_core_bit_t value);

void h_core_bitarray_set_double(h_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, double value);

void h_core_bitarray_set_unsigned_long(h_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, unsigned long value);

void h_core_bitarray_unset_all(h_core_bitarray_t *bitarray);

#endif
