#include "h/core/string.h"
#include "h/core/tools.h"
#include "h/external/external.h"
#include "h/file/basic.h"

#define H_FILE_BASIC_MEMORY_BLOCK_SIZE 4096

static const char *filemode_strings[] = {"r", "w", "a", "r+", "w+", "a+"};

static h_core_bool_t slurp(h_file_basic_t *file, char **blob,
    unsigned long *blob_size);

static char *slurp_string(h_file_basic_t *file);

struct h_file_basic_t {
  FILE *file;
  h_container_list_t *token_list_cache;
};

h_file_basic_t *h_file_basic_create(const char *filename, h_file_mode_t mode)
{
  assert(filename);
  h_file_basic_t *file;
  const char *mode_string;

  file = malloc(sizeof *file);
  if (file) {
    mode_string = filemode_strings[mode];
    file->token_list_cache = NULL;
    file->file = fopen(filename, mode_string);
    if (!file->file) {
      h_core_trace("fopen--%s", filename);
    }
  }

  return file;
}

void h_file_basic_destroy(h_file_basic_t *file)
{
  assert(file);

  if (file->token_list_cache) {
    h_container_list_destroy(file->token_list_cache);
  }

  fclose(file->file);
  free(file);

}

h_core_bool_t h_file_basic_exists(const char *filename)
{
  assert(filename);
  h_core_bool_t exists;
  FILE *file;

  file = fopen(filename, filemode_strings[H_FILE_MODE_OPEN_FOR_READ]);
  if (file) {
    fclose(file);
    exists = h_core_bool_true;
  } else {
    exists = h_core_bool_false;
  }

  return exists;
}

h_core_bool_t h_file_basic_get_as_blob(h_file_basic_t *file, char **blob,
    unsigned long *blob_size)
{
  return slurp(file, blob, blob_size);
}

h_container_list_t *h_file_basic_get_as_line_list(h_file_basic_t *file)
{
  return h_file_basic_get_as_token_list(file, "\n");
}

char *h_file_basic_get_as_string(h_file_basic_t *file)
{
  return slurp_string(file);
}

h_container_list_t *h_file_basic_get_as_token_list(h_file_basic_t *file,
    char *delimiters)
{
  assert(file);
  assert(delimiters);
  h_container_list_t *token_list;
  char *token;
  char *token_copy;
  char *file_string;
  char *strtok_context;

  if (!file->token_list_cache) {
    file->token_list_cache = h_container_list_create(h_core_string_compare,
        h_core_string_copy, h_core_string_destroy);
    if (file->token_list_cache) {
      file_string = slurp_string(file);
      token = strtok_r(file_string, delimiters, &strtok_context);
      while (token) {
        token_copy = strdup(token);
        if (token_copy) {
          if (!h_container_list_add_last(file->token_list_cache, token_copy)) {
            h_core_trace("h_container_list_add_last");
            free(token_copy);
          }
        } else {
          h_core_trace("strdup");
        }
        token = strtok_r(NULL, delimiters, &strtok_context);
      }
      free(file_string);
    } else {
      h_core_trace("h_container_list_create");
    }
  }

  if (file->token_list_cache) {
    token_list = h_container_list_copy(file->token_list_cache);
    if (!token_list) {
      h_core_trace("h_container_list_copy");
    }
  } else {
    token_list = NULL;
  }

  return token_list;
}

FILE *h_file_basic_get_file(h_file_basic_t *file)
{
  return file->file;
}

h_core_bool_t h_file_basic_get_line_count(h_file_basic_t *file,
    unsigned long *line_count)
{
  assert(file);
  h_core_bool_t success;
  int c;

  if (fseek(file->file, 0, SEEK_SET) == 0) {
    success = h_core_bool_true;
    *line_count = 0;
    while ((c = fgetc(file->file)) != EOF) {
      if ('\n' == c) {
        (*line_count)++;
      }
    }
  } else {
    success = h_core_bool_false;
    h_core_trace("");
  }

  return success;
}

h_core_bool_t h_file_basic_write_char(h_file_basic_t *file, int c)
{
  assert(file);
  h_core_bool_t success;

  if (EOF != fputc(c, file->file)) {
    success = h_core_bool_true;
  } else {
    success = h_core_bool_false;
  }

  return success;
}

h_core_bool_t h_file_basic_write_string(h_file_basic_t *file, char *string)
{
  assert(file);
  h_core_bool_t success;
  char *c;

  success = h_core_bool_true;

  c = string;
  while (*c) {
    if (EOF != fputc(*c, file->file)) {
      c++;
    } else {
      success = h_core_bool_false;
      break;
    }
  }

  return success;
}

/*
  TODO: fix the realloc calls _ they don't check the return value properly
*/
h_core_bool_t slurp(h_file_basic_t *file, char **blob,
    unsigned long *blob_size)
{
  assert(file);
  assert(blob);
  assert(blob_size);
  unsigned long characters_read;
  int c;
  h_core_bool_t success;

  *blob_size = 0;
  characters_read = 0;
  *blob = NULL;
  success = h_core_bool_true;

  if (fseek(file->file, 0, SEEK_SET) == 0) {
    while ((c = fgetc(file->file)) != EOF) {
      characters_read++;
      if (characters_read > *blob_size) {
        *blob = realloc(*blob, *blob_size + H_FILE_BASIC_MEMORY_BLOCK_SIZE);
        if (*blob) {
          *blob_size += H_FILE_BASIC_MEMORY_BLOCK_SIZE;
        } else {
          success = h_core_bool_false;
          h_core_trace("realloc");
        }
      }
      if (*blob) {
        *(*blob + (characters_read - 1)) = c;
      }
    }
    *blob = realloc(*blob, characters_read);
    if (!*blob) {
      h_core_trace("realloc");
    }
    *blob_size = characters_read;
  } else {
    success = h_core_bool_false;
    h_core_trace("fseek");
  }

  return success;
}

char *slurp_string(h_file_basic_t *file)
{
  assert(file);
  char *string;
  char *blob;
  unsigned long blob_size;

  if (slurp(file, &blob, &blob_size)) {
    string = malloc(blob_size + 1);
    if (string) {
      memcpy(string, blob, blob_size);
      *(string + blob_size) = '\0';
    } else {
      h_core_trace("malloc");
    }
    free(blob);
  } else {
    string = NULL;
    h_core_trace("malloc");
  }

  return string;
}
