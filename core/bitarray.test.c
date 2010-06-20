#include "h/core/bitarray.h"
#include "h/core/tools.h"
#include "h/external/external.h"

static void test_from_char();
static void test_from_double();
static void test_from_long();
static void test_from_short();
static void test_from_string();
static void test_get_functions();
static void test_set_functions();

int main(int argc, char *argv[])
{
  h_core_bitarray_t *bitarray;

  bitarray = h_core_bitarray_create(4);
  if (!bitarray) {
    h_core_trace_exit("h_core_bitarray_create");
  }

  h_core_bitarray_unset_all(bitarray);
  h_core_bitarray_print(bitarray);
  while (h_core_bitarray_increment(bitarray)) {
    h_core_bitarray_print(bitarray);
  }

  h_core_bitarray_destroy(bitarray);

  test_get_functions();
  test_set_functions();
  test_from_char();
  test_from_double();
  test_from_long();
  test_from_short();
  test_from_string();

  return 0;
}

static void test_from_char()
{
  h_core_bitarray_t *bitarray;
  char *string;

  bitarray = h_core_bitarray_create_from_char('a');
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("a = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);

  bitarray = h_core_bitarray_create_from_char('b');
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("b = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);

  bitarray = h_core_bitarray_create_from_char('c');
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("c = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);
}

static void test_from_double()
{
  h_core_bitarray_t *bitarray;
  char *string;

  bitarray = h_core_bitarray_create_from_double(1.5);
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("1.5 = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);
}

static void test_from_long()
{
  h_core_bitarray_t *bitarray;
  char *string;

  bitarray = h_core_bitarray_create_from_long(1025);
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("1025 = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);
}

static void test_from_short()
{
  h_core_bitarray_t *bitarray;
  char *string;

  bitarray = h_core_bitarray_create_from_short(5);
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("5 = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);
}

static void test_from_string()
{
  h_core_bitarray_t *bitarray = NULL;
  char *string;

  bitarray = h_core_bitarray_create_from_string("A", strlen("A"));
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("A = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);

  bitarray = h_core_bitarray_create_from_string("abc", strlen("abc"));
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("abc = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);

  bitarray = h_core_bitarray_create_from_string("a@#!2323..~",
      strlen("a@#!2323..~"));
  string = h_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("a@#!2323..~ = %s\n", string);
    free(string);
  } else {
    h_core_trace_exit("h_core_bitarray_as_binary_string");
  }
  h_core_bitarray_destroy(bitarray);
}

static void test_get_functions()
{
  double float_value = 0.0;
  unsigned long value = 0;
  h_core_bitarray_t *bitarray = NULL;

  bitarray = h_core_bitarray_create(8);
  if (!bitarray) {
    h_core_trace_exit("h_core_bitarray_create");
  }
  h_core_bitarray_set_all(bitarray);

  value = h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 7);
  if (value != 255) {
    h_core_trace_exit("h_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 255 got %d)", (int) value);
  }

  value = h_core_bitarray_get_unsigned_long_from_bits(bitarray, 2, 4);
  if (value != 7)  {
    h_core_trace_exit("h_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 7 got %d)", (int) value);
  }

  h_core_bitarray_set_bit(bitarray, 0, 0);
  h_core_bitarray_set_bit(bitarray, 2, 0);

  value = h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 3);
  if (value != 10)  {
    h_core_trace_exit("h_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 10 got %d)", (int) value);
  }
  h_core_bitarray_destroy(bitarray);

  bitarray = h_core_bitarray_create(4);
  if (!bitarray) {
    h_core_trace_exit("h_core_bitarray_create");
  }
  h_core_bitarray_set_all(bitarray);

  float_value = h_core_bitarray_get_double_from_bits(bitarray, 0, 3);
  if (float_value != 1.0) {
    h_core_trace_exit("h_core_bitarray_get_doublefrom_bits() "
        "failed (expected 1.0 got %f)", float_value);
  }

  float_value = h_core_bitarray_get_double_from_bits(bitarray, 1, 3);
  if (float_value != 1.0) {
    h_core_trace_exit("h_core_bitarray_get_doublefrom_bits() "
        "failed (expected 1.0 got %f)", float_value);
  }

  h_core_bitarray_set_bit(bitarray, 0, 0);
  h_core_bitarray_set_bit(bitarray, 3, 0);

  float_value = h_core_bitarray_get_double_from_bits(bitarray, 0, 3);
  if (fabs(float_value - (6.0 / 15.0)) > 0.00001) {
    h_core_trace_exit("h_core_bitarray_get_doublefrom_bits() "
        "failed (expected %f got %f)", (6.0 / 15.0), float_value);
  }
  h_core_bitarray_destroy(bitarray);
}

static void test_set_functions()
{
  double value;
  h_core_bitarray_t *bitarray;

  bitarray = h_core_bitarray_create(16);
  h_core_bitarray_set_unsigned_long(bitarray, 0, 15, 31);
  if (h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 31) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_unsigned_long() "
        "failed\n", __FUNCTION__, __LINE__);
  }

  h_core_bitarray_set_unsigned_long(bitarray, 0, 15, 0);
  if (h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 0) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_unsigned_long() "
        "failed\n", __FUNCTION__, __LINE__);
  }

  h_core_bitarray_set_unsigned_long(bitarray, 0, 15, 1);
  if (h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 1) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_unsigned_long() "
        "failed", __FUNCTION__, __LINE__);
  }

  h_core_bitarray_set_unsigned_long(bitarray, 0, 15,
      (unsigned long) (pow(2, 16) - 1));
  if (h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) !=
      (unsigned long) (pow(2, 16) - 1)) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_unsigned_long() "
        "failed (%lu)", __FUNCTION__, __LINE__,
        h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15));
  }

  h_core_bitarray_set_double(bitarray, 0, 15, 0.0);
  value = h_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.0) > 0.001) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  h_core_bitarray_set_double(bitarray, 0, 15, 0.1);
  value = h_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.1) > 0.001) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  h_core_bitarray_set_double(bitarray, 0, 15, 0.33);
  value = h_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.33) > 0.001) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15),
        value);
  }

  h_core_bitarray_set_double(bitarray, 0, 15, 0.5);
  value = h_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.5) > 0.001) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  h_core_bitarray_set_double(bitarray, 0, 15, 0.6478);
  value = h_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.6478) > 0.001) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  h_core_bitarray_set_double(bitarray, 0, 15, 1.0);
  value = h_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 1.0) > 0.001) {
    h_core_trace_exit("[%s():%d] h_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        h_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  h_core_bitarray_destroy(bitarray);
}
