#include "columns.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int initialise_column(Column *column, Array *field)
{
    if (trim_right(field) != 0 || wrap_in_quotes(field) != 0)
    {
        return 1;
    }
    size_t len = strlen(field->ptr) + 1;
    column->name = malloc(len * sizeof(char));
    memcpy(column->name, field->ptr, len * sizeof(char));
    column->type = TYPE_INT;
    column->nulls = 0;
    column->min = +INFINITY;
    column->max = -INFINITY;
    column->sum = 0;
    column->count = 0;
    column->positive = 0;
    column->negative = 0;
    column->even = 0;
    column->integer = 0;

    return 0;
}

void post_init(ColumnsArray *columns)
{
    for (size_t i = 0; i < columns->elts_number; i++)
    {
        Column *column = get_column_ptr(columns, i);
        if (column->count == 0)
        {
            column->mean = 0;
        }

        else
        {
            column->mean = (column->sum) / (column->count);
        }
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

int update_column(ColumnsArray *columns, size_t i, const Array *field)
{
    Column *column = get_column_ptr(columns, i);
    long double number = strtold(field->ptr, NULL);
    Type field_type = get_type(field);

    if (field_type == TYPE_INVALID)
    {
        return 1;
    }

    if (field_type == TYPE_DOUBLE)
    {
        column->type = TYPE_DOUBLE;
    }
    if (field_type == TYPE_NULL)
    {
        column->nulls++;
        return 0;
    }
    column->min = fmin(column->min, number);
    column->max = fmax(column->max, number);
    column->sum += number;
    column->count++;

    if (number > 0)
    {
        column->positive++;
    }
    if (number < 0)
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

int has_empty(ColumnsArray *columns)
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

int has_repeating(ColumnsArray *columns)
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

void print_column_stats(const ColumnsArray *columns, size_t i)
{
    Column *column = get_column_ptr(columns, i);
    printf("%llu\n", i);
    printf("%s\n", column->name);
    printf("%s\n", column->type_str);
    printf("%lld\n", column->nulls);
    if (column->type == TYPE_INT)
    {
        printf("min %.0Lf\n", column->min);
        printf("max %.0Lf\n", column->max);
    }
    else
    {
        printf("min %.6Lf\n", column->min);
        printf("max %.6Lf\n", column->max);
    }
    printf("mean %Lf\n", column->mean);
    printf("positive %lld\n", column->positive);
    printf("negative %lld\n", column->negative);
    if (column->type == TYPE_INT)
    {
        printf("even %lld\n\n", column->even);
    }
    else
    {
        printf("integer %lld\n\n", column->integer);
    }
}

void free_columns(ColumnsArray *columns)
{
    for (size_t i = 0; i < columns->elts_number; i++)
    {
        Column *column = get_column_ptr(columns, i);
        free(column->name);
    }
    free(columns->ptr);
}