#ifndef h_file_csv_h
#define h_file_csv_h

#include "h/container/array.h"
#include "h/core/bit.h"
#include "h/core/object.h"

struct h_file_csv_t;
typedef struct h_file_csv_t h_file_csv_t;

h_file_csv_t *h_file_csv_create(char *filename);

h_file_csv_t *h_file_csv_create_extended(char *filename,
    unsigned long start_object, unsigned long end_object);

void h_file_csv_destroy(void *csv_object);

h_container_array_t *h_file_csv_get_field_by_name_as_array(h_file_csv_t *csv,
    char *field_name);

h_container_array_t *h_file_csv_get_field_by_index_as_array(h_file_csv_t *csv,
    unsigned long field_index);

unsigned long h_file_csv_get_field_count(h_file_csv_t *csv);

h_core_bool_t h_file_csv_get_field_index(h_file_csv_t *csv, char *field_name,
    unsigned long *field_index);

char *h_file_csv_get_field_name(h_file_csv_t *csv, unsigned long field_index);

unsigned long h_file_csv_get_object_count(h_file_csv_t *csv);

h_core_bit_t h_file_csv_get_value_by_index_as_bit(h_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

h_core_bool_t h_file_csv_get_value_by_index_as_bool(h_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char h_file_csv_get_value_by_index_as_char(h_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

double h_file_csv_get_value_by_index_as_double(h_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

long h_file_csv_get_value_by_index_as_long(h_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

short h_file_csv_get_value_by_index_as_short(h_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char *h_file_csv_get_value_by_index_as_string(h_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

unsigned char h_file_csv_get_value_by_index_as_unsigned_char
(h_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned long h_file_csv_get_value_by_index_as_unsigned_long
(h_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned short h_file_csv_get_value_by_index_as_unsigned_short
(h_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

char h_file_csv_get_value_by_name_as_char(h_file_csv_t *csv,
    unsigned long object_index, char *field_name);

double h_file_csv_get_value_by_name_as_double(h_file_csv_t *csv,
    unsigned long object_index, char *field_name);

long h_file_csv_get_value_by_name_as_long(h_file_csv_t *csv,
    unsigned long object_index, char *field_name);

short h_file_csv_get_value_by_name_as_short(h_file_csv_t *csv,
    unsigned long object_index, char *field_name);

char *h_file_csv_get_value_by_name_as_string(h_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned char h_file_csv_get_value_by_name_as_unsigned_char(h_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned long h_file_csv_get_value_by_name_as_unsigned_long(h_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned short h_file_csv_get_value_by_name_as_unsigned_short
(h_file_csv_t *csv, unsigned long object_index, char *field_name);

#endif
