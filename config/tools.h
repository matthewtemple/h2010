#ifndef h_config_tools_h
#define h_config_tools_h

#include "h/core/bool.h"
#include "h/external/external.h"

void h_config_disable_if_running_batch_tests(int argc, char *argv[]);

h_core_bool_t h_config_running_batch_tests(int argc, char *argv[]);

#endif
