#include "h/core/buffer.h"
#include "h/core/constants.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/shell/system.h"

#define INITIAL_IN_PROGRESS_INPUT ""

struct h_core_shell_t {
  h_container_list_t *new_input;
  pthread_mutex_t new_input_mutex;

  h_core_buffer_t *in_progress_input;
  pthread_t shell_thread;
};

static void *shell_thread(void *shell_object);

void h_core_shell_append_output(h_core_shell_t *shell, char *output)
{
  assert(shell);
  assert(output);

  printf("%s", output);
}

h_core_shell_t *h_core_shell_create()
{
  h_core_shell_t *shell;
  h_core_bool_t so_far_so_good;

  shell = malloc(sizeof *shell);
  if (shell) {
    so_far_so_good = h_core_bool_true;
  } else {
    h_core_trace("malloc");
    so_far_so_good = h_core_bool_false;
  }

  if (so_far_so_good) {
    shell->new_input = h_container_list_create(h_core_string_compare,
        h_core_string_copy, H_CORE_NO_DESTROY_FUNCTION);
    if (!shell->new_input) {
      so_far_so_good = h_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (0 != pthread_mutex_init(&shell->new_input_mutex, NULL)) {
      so_far_so_good = h_core_bool_false;
    }
  }

  if (so_far_so_good) {
    shell->in_progress_input = h_core_buffer_create(INITIAL_IN_PROGRESS_INPUT,
        strlen(INITIAL_IN_PROGRESS_INPUT));
    if (!shell->in_progress_input) {
      so_far_so_good = h_core_bool_false;
      h_core_trace("h_core_buffer_create");
    }
  }

  if (!so_far_so_good && shell) {
    if (shell->new_input) {
      h_container_list_destroy(shell->new_input);
    }
    if (shell->in_progress_input) {
      h_core_buffer_destroy(shell->in_progress_input);
    }
    free(shell);
    shell = NULL;
  }

  return shell;
}

void h_core_shell_destroy(h_core_shell_t *shell)
{
  assert(shell);

  pthread_join(shell->shell_thread, NULL);
  h_container_list_destroy(shell->new_input);
  pthread_mutex_destroy(&shell->new_input_mutex);
  h_core_buffer_destroy(shell->in_progress_input);
  free(shell);

  h_core_terminal_block();
}

h_core_bool_t h_core_shell_start(h_core_shell_t *shell)
{
  assert(shell);
  h_core_bool_t success;

  h_core_terminal_nonblock();

  if (0 == pthread_create(&shell->shell_thread, NULL, shell_thread, shell)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

h_container_list_t *h_core_shell_take_input(h_core_shell_t *shell)
{
  assert(shell);
  h_container_list_t *new_input;
  char *input_string;

  new_input = h_container_list_create(h_core_string_compare,
      h_core_string_copy, h_core_string_destroy);
  if (new_input) {

    pthread_mutex_lock(&shell->new_input_mutex);
    {
      h_container_list_iterate_start(shell->new_input);
      while ((input_string
              = h_container_list_iterate_next(shell->new_input))) {
        if (h_container_list_add_last(new_input, input_string)) {
          h_container_list_iterate_remove(shell->new_input);
        } else {
          h_core_trace("h_container_list_add_last");
        }
      }
    }
    pthread_mutex_unlock(&shell->new_input_mutex);

  } else {
    h_core_trace("h_container_list_create");
  }

  return new_input;
}

void *shell_thread(void *shell_object)
{
  assert(shell_object);
  h_core_shell_t *shell;
  int c;
  char *input_string;
  unsigned long input_string_size;

  shell = shell_object;

  while (!h_core_stop_requested) {
    if (h_core_key_hit()) {
      c = getchar();
      if ('\n' == c) {
        input_string = h_core_buffer_take_string(shell->in_progress_input,
            &input_string_size);
        if (input_string) {

          pthread_mutex_lock(&shell->new_input_mutex);
          {
            if (!h_container_list_add_last(shell->new_input, input_string)) {
              h_core_trace("h_container_list_add_last");
            }
          }
          pthread_mutex_unlock(&shell->new_input_mutex);

        }
      } else {
        h_core_buffer_append_char(shell->in_progress_input, c);
      }
    }
    usleep(H_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  return NULL;
}
