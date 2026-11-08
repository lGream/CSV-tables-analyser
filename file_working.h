#pragma once
#include <stdio.h>
#pragma once
#include <stdlib.h>
#pragma once
#include <ctype.h>
#pragma once
#include <string.h>
#pragma once
#include <math.h>
#pragma once
#include "array_stuff.h"
#pragma once
#include "types_determination.h"

#define ERROR_MEMORY_ALLOCATION 1
#define EMPTY_LINE 2
#define WRONG_INPUT 3

struct Column
{
    const char *name;
    ValueType type;
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
};

int is_space(char c)
{
    return c == ' ' || c == '\t';
}

int is_delimiter(char c)
{
    return c == ',';
}

int initialise_column(int i; struct Column **columns, int column_count, char *field)
{
    char *buffer;
    if (wrap_in_quotes(field[i], &buffer) != 0)
    {
        return 1;
    }

    (*columns)[i].name = buffer;
    free(buffer);
    (*columns)[i].type = INT;
    (*columns)[i].min = +INFINITY;
    (*columns)[i].max = -INFINITY;
    (*columns)[i].sum = 0;
    (*columns)[i].count = 0;
    (*columns)[i].even = 0;
    (*columns)[i].integer = 0;

    return 0;
}

int update_column(int i, char *field, struct Column **columns)
{
    ValueType field_type = type(field);
    ValueType column_type = (*columns)[i].type;
    struct Column column = (*columns)[i];
    long double number = strtold(field, NULL);

    if (field_type == DOUBLE)
    {
        (*columns)[i].type = DOUBLE;
    }

    (*columns)[i].min = fmin(column.min, number);
    (*columns)[i].max = fmax(column.min, number);
    (*columns)[i].sum += number;
    (*columns)[i].count++;

    if (number > 0)
    {
        (*columns)[i].positive++;
    }
    if (number < 0)
    {
        (*columns)[i].negative++;
    }
    if (number == truncl(number))
    {
        (*columns)[i].integer++;
    }
    if (number == truncl(number) && fmodl(number, 2.0L) == 0)
    {
        (*columns)[i].even++;
    }

    return 0;
}

int read_line(FILE *file, char ***fields, int *columns_count, struct Column **columns, int is_header)
{
    if (*fields == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    if (is_header)
        *columns_count = 0;

    int fields_count = 0;
    int fields_capacity = 1;

    int field_length = 0;
    int field_capacity = 1;
    char *field = malloc(field_capacity * sizeof(char));
    char c;

    while (1)
    {
        c = getc(file);
        int is_end_of_line = c == '\n' || c == EOF;

        if (is_space(c) && field_length == 0)
            continue;

        if (!is_delimiter(c) && !is_end_of_line)
        {
            field_length++;
            if (expand_string(&field, &field_capacity, field_length) != 0)
            {
                return ERROR_MEMORY_ALLOCATION;
            }
            field[field_length - 1] = c;
            continue;
        }

        // Поле закончилось
        field[field_length] = '\0';
        if (!is_header && fields_count + 1 > *columns_count)
            return 4;
        update_column(*columns_count, field, columns);
        fields_count++;
        if (expand_array_of_strings(fields, &fields_capacity, fields_count) != 0)
            return ERROR_MEMORY_ALLOCATION;

        (*fields)[fields_count - 1] = field;

        if (reset_string(&field, &field_capacity, &field_length) != 0)
            return ERROR_MEMORY_ALLOCATION;

        if (c == EOF)
            return EOF;

        if (is_end_of_line)
            break;
    }

    if (is_header)
        *columns_count = fields_count;
    if (!is_header && fields_count != *columns_count)
        return 5;
    if (fields_count == 0)
        return EMPTY_LINE;

    return 0;
}

#if 0
int read_csv(FILE *file, int *columns_count, struct Column **columns)
{
    char **fields = malloc(1 * sizeof(char *));
    if (read_line(file, &fields, columns_count, , 1) != 0)
        return 1;
    if (has_repeats(fields, *columns_count))
        return WRONG_INPUT;

    for (int i = 0; i < columns_count; i++)
    {
        (*columns)[i].name = malloc((strlen(fields[i]) + 2) * sizeof(char));
        (*columns)[i].name[0] = "\"";
        for (int j = 1; j < strlen(fields[i]) + 1; j++)
        {
            (*columns)[i].name[j] = field[j - 1];
        }
        (*columns)[i].type = INT;
    }

    free(fields);
    fields = malloc(1 * sizeof(char *));
    int code;
    while (code = read_line(file, &fields, columns_count, types, 0) != END_OF_FILE)
    {
        for (int i = 0; i < columns_count; i++)
        {
        }
    }
}
#endif