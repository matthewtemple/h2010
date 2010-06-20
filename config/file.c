#include "h/config/file.h"
#include "h/container/set.h"
#include "h/core/nameobject.h"
#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/file/basic.h"

struct h_config_file_t {
  /* TODO: reimplement with h_container_map_t, like config_options? */
  h_container_set_t *strings;
  h_container_set_t *string_lists;
};

static h_core_bool_t find_as_string(h_config_file_t *file, char *name,
    char **value);

static void h_config_file_create_rollback(h_config_file_t *conf);

static h_core_bool_t line_is_a_comment(char *line);

static void parse_list_value(h_config_file_t *file, char *name, char *value);

static void parse_string_value(h_config_file_t *file, char *name, char *value);

static void read_file(h_config_file_t *file, char *filename);

static h_core_bool_t value_is_a_list(char *name);

h_core_bool_t find_as_string(h_config_file_t *file, char *name,
    char **value)
{
  assert(file);
  assert(name);
  assert(value);
  h_core_nameobject_t *nameobject_decoy;
  h_core_nameobject_t *nameobject;
  h_core_bool_t found_it;

  *value = NULL;

  nameobject_decoy = h_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = h_container_set_find(file->strings, nameobject_decoy);
    h_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      found_it = h_core_bool_true;
      *value = h_core_nameobject_get_object(nameobject);
    } else {
      found_it = h_core_bool_false;
    }
  } else {
    found_it = h_core_bool_false;
    h_core_trace("h_core_nameobject_create_decoy");
  }

  return found_it;
}

h_config_file_t *h_config_file_create(char *filename)
{
  h_config_file_t *file;
  h_core_bool_t so_far_so_good;

  file = malloc(sizeof *file);
  if (file) {
    so_far_so_good = h_core_bool_true;
  } else {
    so_far_so_good = h_core_bool_false;
  }

  if (so_far_so_good) {
    file->strings = h_container_set_create(h_core_nameobject_compare,
        h_core_nameobject_copy, h_core_nameobject_destroy);
    if (!file->strings) {
      so_far_so_good = h_core_bool_false;
    }
  }

  if (so_far_so_good) {
    file->string_lists = h_container_set_create(h_core_nameobject_compare,
        h_core_nameobject_copy, h_core_nameobject_destroy);
    if (!file->string_lists) {
      so_far_so_good = h_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (h_file_basic_exists(filename)) {
      read_file(file, filename);
    }
  }

  if (!so_far_so_good && file) {
    h_config_file_create_rollback(file);
    file = NULL;
  }

  return file;
}

void h_config_file_create_rollback(h_config_file_t *file)
{
  assert(file);

  if (file->strings) {
    h_container_set_destroy(file->strings);
  }
  if (file->string_lists) {
    h_container_set_destroy(file->string_lists);
  }
  free(file);

}

void h_config_file_destroy(h_config_file_t *file)
{
  assert(file);
  assert(file->strings);
  assert(file->string_lists);

  h_container_set_destroy(file->strings);
  h_container_set_destroy(file->string_lists);
  free(file);

}

h_core_bool_t h_config_file_find(h_config_file_t *file, char *name)
{
  assert(file);
  assert(name);
  h_core_nameobject_t *nameobject_decoy;
  h_core_bool_t found_it;

  nameobject_decoy = h_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    if (h_container_set_find(file->strings, nameobject_decoy)) {
      found_it = h_core_bool_true;
    } else {
      found_it = h_core_bool_false;
    }
    h_core_nameobject_destroy_decoy(nameobject_decoy);
  } else {
    found_it = h_core_bool_false;
    h_core_trace("h_core_nameobject_create_decoy");
  }

  return found_it;
}

h_core_bool_t h_config_file_find_as_double(h_config_file_t *file, char *name,
    double *value, double default_value)
{
  assert(file);
  assert(name);
  assert(value);
  h_core_bool_t found;
  char *value_string;

  if (find_as_string(file, name, &value_string)) {
    found = h_core_bool_true;
    *value = atof(value_string);
  } else {
    found = h_core_bool_false;
    *value = default_value;
  }

  return found;
}

h_core_bool_t h_config_file_find_as_string(h_config_file_t *file, char *name,
    char **value, char *default_value)
{
  assert(file);
  assert(name);
  assert(value);
  assert(default_value);
  h_core_bool_t found_it;

  if (find_as_string(file, name, value)) {
    found_it = h_core_bool_true;
  } else {
    found_it = h_core_bool_false;
    *value = default_value;
  }

  return found_it;
}

h_core_bool_t h_config_file_find_as_unsigned_long(h_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(file);
  assert(name);
  assert(value);
  char *value_string;
  h_core_bool_t found;

  if (find_as_string(file, name, &value_string)) {
    found = h_core_bool_true;
    *value = atol(value_string);
  } else {
    found = h_core_bool_false;
    *value = default_value;
  }

  return found;
}

h_core_bool_t h_config_file_find_as_unsigned_short(h_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(file);
  assert(name);
  assert(value);
  unsigned long value_unsigned_long;
  h_core_bool_t success;

  success = h_config_file_find_as_unsigned_long
    (file, name, &value_unsigned_long, default_value);
  if (success) {
    *value = (unsigned short) value_unsigned_long;
  }

  return success;
}

h_core_bool_t h_config_file_find_list_as_strings(h_config_file_t *file,
    char *name, h_container_list_t **list)
{
  assert(file);
  assert(name);
  h_core_nameobject_t *nameobject_decoy;
  h_core_nameobject_t *nameobject;
  h_core_bool_t success;

  *list = NULL;

  nameobject_decoy = h_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = h_container_set_find(file->string_lists, nameobject_decoy);
    h_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      success = h_core_bool_true;
      *list = h_core_nameobject_get_object(nameobject);
    } else {
      success = h_core_bool_false;
    }
  } else {
    success = h_core_bool_false;
    h_core_trace("h_core_nameobject_create_decoy");
  }

  return success;
}

h_core_bool_t line_is_a_comment(char *line)
{
  assert(line);
  h_core_bool_t is_a_comment;

  if (strlen(line) > 0) {
    if ('#' == *line) {
      is_a_comment = h_core_bool_true;
    } else {
      is_a_comment = h_core_bool_false;
    }
  } else {
    is_a_comment = h_core_bool_true;
  }

  return is_a_comment;
}

void parse_list_value(h_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  h_core_nameobject_t *nameobject;
  h_container_list_t *list;
  char *string;
  char *string_copy;
  char *strtok_context;

  list = h_container_list_create(h_core_string_compare, h_core_string_copy,
      h_core_string_destroy);
  if (list) {
    string = strtok_r(value, ",", &strtok_context);
    while (string) {
      string_copy = strdup(string);
      if (string_copy) {
        if (!h_container_list_add_last(list, string_copy)) {
          h_core_trace("h_container_list_add_last");
        }
      } else {
        h_core_trace("strdup");
      }
      string = strtok_r(NULL, ",", &strtok_context);
    }
    nameobject = h_core_nameobject_create(name, list, H_CORE_NO_COPY_FUNCTION,
        h_container_list_destroy, h_container_list_get_as_string);
    if (nameobject) {
      if (!h_container_set_add(file->string_lists, nameobject)) {
        h_core_trace("h_container_set_add");
        h_core_nameobject_destroy(nameobject);
      }
    } else {
      h_core_trace("h_core_nameobject_create");
    }
  } else {
    h_core_trace("h_container_list_create");
  }

}

void parse_string_value(h_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  h_core_nameobject_t *nameobject;

  nameobject = h_core_nameobject_create(name, value, h_core_string_copy,
      h_core_string_destroy, h_core_string_get_as_string);
  if (nameobject) {
    if (!h_container_set_add(file->strings, nameobject)) {
      h_core_trace("h_container_set_add");
    }
  } else {
    h_core_trace("h_core_nameobject_create");
  }

}

void read_file(h_config_file_t *file, char *filename)
{
  assert(file);
  assert(filename);
  h_file_basic_t *file_basic;
  h_container_list_t *lines;
  char *line;
  char *name;
  char *value;
  char *strtok_context;

  file_basic = h_file_basic_create(filename, H_FILE_MODE_OPEN_FOR_READ);
  if (file_basic) {
    lines = h_file_basic_get_as_line_list(file_basic);
    if (lines) {
      h_container_list_iterate_start(lines);
      while ((line = h_container_list_iterate_next(lines))) {
        if (!line_is_a_comment(line)) {
          name = strtok_r(line, ":", &strtok_context);
          if (name) {
            value = strtok_r(NULL, "\n", &strtok_context);
            if (!value) {
              value = "";
            }
            if (value_is_a_list(name)) {
              parse_list_value(file, name, value);
            } else {
              parse_string_value(file, name, value);
            }
          }
        }
      }
      h_container_list_destroy(lines);
    }
    h_file_basic_destroy(file_basic);
  } else {
    h_core_trace("h_core_file_create");
  }
}

h_core_bool_t value_is_a_list(char *name)
{
  assert(name);
  h_core_bool_t is_a_list;
  unsigned short name_size;
  unsigned short start_compare_position;

  name_size = strlen(name);

  if (name_size >= 6) {
    start_compare_position = name_size - 6;
    if (0 == strcmp("[list]", name + start_compare_position)) {
      is_a_list = h_core_bool_true;
    } else {
      is_a_list = h_core_bool_false;
    }
  } else {
    is_a_list = h_core_bool_false;
  }

  return is_a_list;
}
