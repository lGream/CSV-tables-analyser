#pragma once

#include <stdio.h>

typedef struct Array
{
    void *ptr;
    size_t capacity;
    size_t elts_number;
    size_t elt_size;
} Array;

int is_space(char c);

int is_delimiter(char c);

int initialise(Array *array, size_t elt_size);

int expand(Array *array, size_t new_elts_number);

int append(Array *array, void *elt);

int clear(Array *array);

int append_char(Array *array, char c);

int append_string(Array *array, const char *string);

int wrap_in_quotes(Array *string);

int trim_right(Array *string);
