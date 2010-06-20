#ifndef h_shell_system_h
#define h_shell_system_h

#include "h/container/list.h"

struct h_core_shell_t;
typedef struct h_core_shell_t h_core_shell_t;

void h_core_shell_append_output(h_core_shell_t *shell, char *output);

h_core_shell_t *h_core_shell_create();

void h_core_shell_destroy(h_core_shell_t *shell);

h_core_bool_t h_core_shell_start(h_core_shell_t *shell);

h_container_list_t *h_core_shell_take_input(h_core_shell_t *shell);

#endif
