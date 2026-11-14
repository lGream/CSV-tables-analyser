#pragma once

#include "arrays.h"
#include "types.h"

typedef struct Column
{
    char *name;
    Type type;
    char *type_str;
    long long nulls;
    long double min;
    long double max;
    long double sum;
    long double mean;
    long long count;
    long long positive;
    long long negative;
    long long even;
    long long integer;
} Column;

typedef struct ColumnsArray
{
    Column *ptr;
    size_t capacity;
    size_t elts_number;
    size_t elt_size;
} ColumnsArray;

Column *get_column_ptr(const ColumnsArray *columns, size_t i);

int initialise_columns_arr(ColumnsArray *columns);

int initialise_column(Column *column, Array *field);

void post_init(ColumnsArray *columns);

int append_column(ColumnsArray *columns, const Column *column);

int update_column(ColumnsArray *columns, size_t i, const Array *field);

int has_empty(ColumnsArray *columns);

int has_repeating(ColumnsArray *columns);

void print_column_stats(const ColumnsArray *columns, size_t i);

void free_columns(ColumnsArray *columns);
