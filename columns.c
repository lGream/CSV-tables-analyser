#include "columns.h"
#include "types.h"
#include "chars.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void remove_minus_if_zero(long double *x);

long long min(long long a, long long b);

long long max(long long a, long long b);

Column *get_column_ptr(const ColumnsArray *columns, size_t i)
{
    return &(columns->ptr[i]);
}

int initialise_columns_arr(ColumnsArray *columns)
{
    if (initialise((Array *)columns, sizeof(Column)) != 0)
    {
        return 1;
    }
    return 0;
}

int initialise_column(Column *column, const Array *field)
{
    size_t len = strlen(field->ptr) + 1;
    column->name = malloc(len * sizeof(char));

    if (column->name == NULL)
    {
        return 1;
    }

    memcpy(column->name, field->ptr, len * sizeof(char));
    column->type = TYPE_INT;
    column->nulls = 0;

    column->int_min = INT_MAX;
    column->int_max = INT_MIN;
    column->int_sum = 0;

    column->min = +INFINITY;
    column->max = -INFINITY;
    column->sum = 0.0L;

    column->count_nonnull = 0;
    column->count_total = 0;
    column->positive = 0;
    column->negative = 0;
    column->even = 0;
    column->integer = 0;

    return 0;
}

int update_column(ColumnsArray *columns, size_t i, const Array *field)
{
    Column *column = get_column_ptr(columns, i);
    long double number = strtold(field->ptr, NULL);
    Type field_type = get_type(field);

    if (field_type == TYPE_INVALID)
    {
        return 1;
    }

    column->count_total++;

    if (field_type == TYPE_DOUBLE)
    {
        column->type = TYPE_DOUBLE;
    }
    if (field_type == TYPE_NULL)
    {
        column->nulls++;
        return 0;
    }

    if (column->type == TYPE_INT)
    {
        column->int_min = min((long long)number, column->int_min);
        column->int_max = max((long long)number, column->int_max);
        column->int_sum += (long long)number;
    }

    column->min = fmin(column->min, number);
    column->max = fmax(column->max, number);
    column->sum += number;
    column->count_nonnull++;

    if (number > 0L)
    {
        column->positive++;
    }
    if (number < 0L)
    {
        column->negative++;
    }
    if (number == truncl(number))
    {
        column->integer++;
    }
    if (number == truncl(number) && fmodl(number, 2.0) == 0)
    {
        column->even++;
    }

    return 0;
}

void post_init(ColumnsArray *columns)
{
    for (size_t i = 0; i < columns->elts_number; i++)
    {
        Column *column = get_column_ptr(columns, i);
        long double sum = column->type == TYPE_INT ? column->int_sum : column->sum;
        column->mean = column->count_nonnull != 0 ? (sum) / (column->count_nonnull) : +0.0L;
        remove_minus_if_zero(&(column->mean));
        remove_minus_if_zero(&(column->sum));
        remove_minus_if_zero(&(column->min));
        remove_minus_if_zero(&(column->max));

        switch (column->type)
        {
        case TYPE_INT:
            column->type_str = "INT";
            break;
        case TYPE_DOUBLE:
            column->type_str = "DOUBLE";
            break;
        default:
            column->type_str = "INVALID";
            break;
        }
    }
}

int append_column(ColumnsArray *columns, const Column *column)
{
    return append((Array *)columns, (void *)column);
}

int has_empty(const ColumnsArray *columns)
{
    for (size_t i = 0; i < columns->elts_number; i++)
    {
        Column *column = get_column_ptr(columns, i);
        if (strlen(column->name) < 2)
        {
            return 1;
        }
    }
    return 0;
}

int has_repeating(const ColumnsArray *columns)
{
    size_t columns_count = columns->elts_number;
    for (size_t i = 0; i < columns_count; i++)
    {
        for (size_t j = i + 1; j < columns_count; j++)
        {
            Column *column1 = get_column_ptr(columns, i);
            Column *column2 = get_column_ptr(columns, j);
            if (strcmp(column1->name, column2->name) == 0)
                return 1;
        }
    }
    return 0;
}

void free_columns(ColumnsArray *columns)
{
    for (size_t i = 0; i < columns->elts_number; i++)
    {
        Column *column = get_column_ptr(columns, i);
        if (column->name != NULL)
        {
            free(column->name);
        }
    }
    if (columns->ptr != NULL)
    {
        free(columns->ptr);
    }
}

void remove_minus_if_zero(long double *x)
{
    if (*x == -0.0L)
    {
        *x = 0.0L;
    }
}

long long min(long long a, long long b)
{
    return a < b ? a : b;
}

long long max(long long a, long long b)
{
    return a > b ? a : b;
}