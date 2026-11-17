#include "columns.h"
#include "types.h"
#include <math.h>
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
    column->min = +INFINITY;
    column->max = -INFINITY;
    column->sum = 0;
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
        column->mean = column->count_nonnull != 0 ? (column->sum) / (column->count_nonnull) : 0;

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
