#include "h/core/bool.h"
#include "h/core/tools.h"
#include "h/core/types.h"
#include "h/external/external.h"

h_core_bool_t h_core_stop_requested;

unsigned short h_core_coin_toss()
{
  return random() % 2;
}

void h_core_do_nothing() {}

h_core_bool_t h_core_get_current_time_string(h_core_time_string_t time_string)
{
  assert(time_string);
  struct timeval time_value;
  struct tm time_elements;
  h_core_bool_t success;
  unsigned int microseconds;

  if (gettimeofday(&time_value, NULL) != -1) {
    if (localtime_r(&time_value.tv_sec, &time_elements)) {
      microseconds = time_value.tv_usec;
      if (H_CORE_TIME_STRING_LENGTH == snprintf(time_string,
              H_CORE_TIME_STRING_LENGTH + 1, "%02i:%02i:%02i:%06i",
              time_elements.tm_hour, time_elements.tm_min,
              time_elements.tm_sec, microseconds)) {
        success = h_core_bool_true;
      } else {
        success = h_core_bool_false;
        h_core_trace("snprintf");
      }
    } else {
      success = h_core_bool_false;
      h_core_trace("localtime_r");
    }
  } else {
    success = h_core_bool_false;
    h_core_trace("gettimeofday");
  }

  return success;
}

double h_core_gray_coin_toss()
{
  return ((double) random()) / (random());
}

unsigned long h_core_hash_djb2(char *string)
{
  assert(string);
  unsigned long hash;
  int c;

  hash = 5381;

  while ((c = *string++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}

unsigned long h_core_hash_djb2_xor(char *string)
{
  assert(string);
  unsigned long hash;
  int c;

  hash = 5381;

  while ((c = *string++)) {
    hash = ((hash << 5) + hash) ^ c;
  }

  return hash;
}

unsigned long h_core_hash_sdbm(char *string)
{
  assert(string);
  unsigned long hash;
  int c;

  hash = 0;

  while ((c = *string++)) {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}

h_core_bool_t h_core_key_hit()
{
  struct timeval tv;
  fd_set fds;
  h_core_bool_t hit;

  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);

  if (FD_ISSET(STDIN_FILENO, &fds)) {
    hit = h_core_bool_true;
  } else {
    hit = h_core_bool_false;
  }

  return hit;
}

void h_core_note_maximum_double(double *maximum, double candidate)
{
  assert(maximum);

  if (candidate > *maximum) {
    *maximum = candidate;
  }
}

void h_core_note_maximum_unsigned_long(unsigned long *maximum,
    unsigned long candidate)
{
  assert(maximum);

  if (candidate > *maximum) {
    *maximum = candidate;
  }
}

double h_core_percentage(double part, double whole)
{
  double percent;

  if (0 != whole) {
    percent = part / whole;
  } else {
    percent = 0.0;
  }

  return percent;
}

void h_core_request_stop()
{
  h_core_stop_requested = h_core_bool_true;
}

void h_core_seed_random(unsigned long seed)
{
  srandom(seed);
}

void h_core_seed_random_with_time()
{
  srandom(time(NULL));
}

void h_core_set_bit_in_unsigned_char(unsigned char *c,
    unsigned short bit_index, h_core_bit_t bit_value)
{
  assert(bit_index < 8);

  if (bit_value) {
    *c |= (1 << bit_index);
  } else {
    *c &= ~(1 << bit_index);
  }
}

void h_core_show_memory(void *location, unsigned short length)
{
  char *char_location;
  char c;
  unsigned short each_char;

  char_location = location;

  for (each_char = 0; each_char < length; each_char++) {
    c = *(char_location + each_char);
    if (isprint((int) c)) {
      printf("%c", c);
    } else {
      printf("[%d]", c);
    }
  }
}

char *h_core_string_append(char *original, const char *addition)
{
  assert(addition);
  char *new_string;
  unsigned long addition_size;
  unsigned long original_size;
  unsigned long new_string_size;

  addition_size = strlen(addition);

  if (original) {
    original_size = strlen(original);
    new_string_size = original_size + addition_size;
    new_string = malloc(new_string_size + 1);
    if (new_string) {
      memcpy(new_string, original, original_size);
      memcpy(new_string + original_size, addition, addition_size);
      *(new_string + new_string_size) = '\0';
    } else {
      h_core_trace("malloc");
    }
    free(original);
  } else {
    new_string = malloc(addition_size + 1);
    if (new_string) {
      memcpy(new_string, addition, addition_size);
      *(new_string + addition_size) = '\0';
    } else {
      h_core_trace("malloc");
    }
  }

  return new_string;
}

char *h_core_string_append_char(char *original, char addition)
{
  assert(addition);
  char *new_string;
  unsigned long original_size;
  unsigned long new_string_size;

  if (original) {
    original_size = strlen(original);
    new_string_size = original_size + 1;
    new_string = malloc(new_string_size + 1);
    if (new_string) {
      memcpy(new_string, original, original_size);
      *(new_string + original_size) = addition;
      *(new_string + new_string_size) = '\0';
    } else {
      h_core_trace("malloc");
    }
    free(original);
  } else {
    new_string = malloc(1 + 1);
    if (new_string) {
      *(new_string) = addition;
      *(new_string + 1) = '\0';
    } else {
      h_core_trace("malloc");
    }
  }

  return new_string;
}

char *h_core_string_append_multiple(char *original, ...)
{
  va_list ap;
  char *addition;
  char *new_string;

  new_string = original;

  va_start(ap, original);
  while ((addition = va_arg(ap, char *))) {
    new_string = h_core_string_append(new_string, addition);
    if (!new_string) {
      h_core_trace("h_core_string_append");
    }
  }
  va_end(ap);

  return new_string;
}

char *h_core_string_append_n(char *original, char *addition,
    unsigned long addition_size)
{
  assert(addition);
  char *new_string;
  unsigned long original_size;
  unsigned long new_string_size;

  if (original) {
    original_size = strlen(original);
    new_string_size = original_size + addition_size;
    new_string = malloc(new_string_size + 1);
    if (new_string) {
      memcpy(new_string, original, original_size);
      memcpy(new_string + original_size, addition, addition_size);
      *(new_string + new_string_size) = '\0';
    } else {
      h_core_trace("malloc");
    }
    free(original);
  } else {
    new_string = malloc(addition_size + 1);
    if (new_string) {
      memcpy(new_string, addition, addition_size);
      *(new_string + addition_size) = '\0';
    } else {
      h_core_trace("malloc");
    }
  }

  return new_string;
}

char *h_core_substring(char *string, unsigned short max_length)
{
  assert(string);
  return NULL;
}

void h_core_terminal_block()
{
  struct termios ttystate;

  tcgetattr(STDIN_FILENO, &ttystate);
  ttystate.c_lflag |= ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

void h_core_terminal_nonblock()
{
  struct termios ttystate;

  tcgetattr(STDIN_FILENO, &ttystate);
  ttystate.c_lflag &= ~ICANON;
  ttystate.c_cc[VMIN] = 1;
  tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

h_core_bool_t h_core_time_is_remaining_microseconds(struct timeval *start_time,
    unsigned long max_wall_time_microseconds)
{
  assert(start_time);
  struct timeval current_time;
  int is_remaining;
  unsigned long elapsed_microseconds;

  gettimeofday(&current_time, NULL);
  elapsed_microseconds
    = (1000000 * (current_time.tv_sec - start_time->tv_sec))
    + (current_time.tv_usec - start_time->tv_usec);

  if (elapsed_microseconds >= max_wall_time_microseconds) {
    is_remaining = 0;
  } else {
    is_remaining = 1;
  }

  return is_remaining;
}

void h_core_truncate_string(char *string, unsigned short max_length)
{
  assert(string);
  unsigned long string_length;

  string_length = strlen(string);
  if (max_length < string_length) {
    *(string + max_length) = '\0';
  }
}

unsigned long h_core_wrap_index(long virtual_index, unsigned long range)
{
  unsigned long wrapped_index;

  if (virtual_index >= (long) range) {
    wrapped_index = virtual_index % range;
  } else if (virtual_index < 0) {
    if (1 == range) {
      wrapped_index = 0;
    } else {
      wrapped_index = range - ((-1 * virtual_index) % range);
    }
  } else {
    wrapped_index = virtual_index;
  }

  return wrapped_index;
}
