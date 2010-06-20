#ifndef h_core_tools_h
#define h_core_tools_h

#include "h/core/bit.h"
#include "h/core/types.h"
#include "h/external/external.h"

void h_core_do_nothing();

#define h_core_get_bit(value, bit_index) ((value >> bit_index) & 1)

#define h_core_hash(string) h_core_hash_djb2_xor(string)

#define h_core_max(a, b) ((a > b) ? a : b)

#define h_core_min(a, b) ((a < b) ? a : b)

#define h_core_trace(message, ...) fprintf(stderr, message "() in %s:%d\n", \
      ##__VA_ARGS__, __FILE__, __LINE__)

#define h_core_trace_exit(message, ...) fprintf                         \
  (stderr, message"() in %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__);  \
  exit(22);

extern h_core_bool_t h_core_stop_requested;

/*
  TODO: rename h_core_toss_coin()
*/
unsigned short h_core_coin_toss();

h_core_bool_t h_core_get_current_time_string(h_core_time_string_t time_string);

double h_core_gray_coin_toss();

unsigned long h_core_hash_djb2(char *string);

unsigned long h_core_hash_djb2_xor(char *string);

unsigned long h_core_hash_sdbm(char *string);

h_core_bool_t h_core_key_hit();

void h_core_note_maximum_double(double *maximum, double candidate);

void h_core_note_maximum_unsigned_long(unsigned long *maximum,
    unsigned long candidate);

double h_core_percentage(double part, double whole);

void h_core_request_stop();

void h_core_seed_random(unsigned long seed);

void h_core_seed_random_with_time();

void h_core_set_bit_in_unsigned_char(unsigned char *c,
    unsigned short bit_index, h_core_bit_t bit_value);

void h_core_show_memory(void *location, unsigned short length);

char *h_core_string_append(char *original, const char *addition);

char *h_core_string_append_char(char *original, char addition);

char *h_core_string_append_multiple(char *original, ...);

char *h_core_string_append_n(char *original, char *addition,
    unsigned long addition_size);

char *h_core_substring(char *string, unsigned short max_length);

void h_core_terminal_block();

void h_core_terminal_nonblock();

/*!
 * determine if time is remaining, or has elapsed.
 *
 * \param start_time pointer to an already-initialized timeval containing the
 *        start time.
 * \param max_wall_time_microseconds the number of microseconds after which
 *        this function will consider that time is not remaining.
 *
 * \return whether or not time is remaining
 */
h_core_bool_t h_core_time_is_remaining_microseconds(struct timeval *start_time,
    unsigned long max_wall_time_microseconds);

void h_core_truncate_string(char *string, unsigned short max_length);

unsigned long h_core_wrap_index(long virtual_index, unsigned long range);

#endif
