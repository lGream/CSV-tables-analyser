#pragma once

#include "arrays.h"
#include "types.h"

typedef struct Column
{
    char *name;
    Type type;
    char *type_str;
    size_t nulls;
    long double min;
    long double max;
    long double sum;
    long double mean;
    size_t count_nonnull;
    size_t count_total;
    size_t positive;
    size_t negative;
    size_t even;
    size_t integer;
} Column;

typedef struct ColumnsArray
{
    Column *ptr;
    size_t capacity;
    size_t elts_number;
    size_t elt_size;
} ColumnsArray;

Column *get_column_ptr(const ColumnsArray *columns, size_t i);

int initialise_column(Column *column, const Array *field);

int update_column(ColumnsArray *columns, size_t i, const Array *field);

int initialise_columns_arr(ColumnsArray *columns);

void post_init(ColumnsArray *columns);

int append_column(ColumnsArray *columns, const Column *column);

int has_empty(const ColumnsArray *columns);

int has_repeating(const ColumnsArray *columns);

void free_columns(ColumnsArray *columns);
