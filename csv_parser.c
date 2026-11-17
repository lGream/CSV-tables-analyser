#include "csv_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef enum
{
    CSV_OK = 0,
    CSV_EOF = 1,
    CSV_EMPTY_LINE = 2,
    CSV_EMPTY_EOF_LINE = 3,

    CSV_ERROR_MEMORY = 101,
    CSV_ERROR_FORMAT = 102,
    CSV_ERROR_EMPTY_FILE = 103,
    CSV_ERROR_EMPTY_LINE_NOT_LAST = 104,
    CSV_ERROR_REPEATING_COLUMNS = 105,
    CSV_ERROR_FIELDS_NUMBER = 106
} ParseCsvStatus;

ParseCsvStatus parse_csv(FILE *file, ColumnsArray *columns, size_t *error_line);

ParseCsvStatus parse_line(FILE *file, ColumnsArray *columns, Array *field, int is_header);

ParseCsvStatus parse_record(FILE *file, ColumnsArray *columns, Array *field);

ParseCsvStatus parse_header(FILE *file, ColumnsArray *columns, Array *field);

ExitCode run_csv_parser(FILE *file, ColumnsArray *columns)
{
    size_t error_line;
    ParseCsvStatus status = parse_csv(file, columns, &error_line);
    if (status == CSV_OK)
        return EXIT_OK;
    else if (status == CSV_ERROR_EMPTY_FILE)
        fprintf(stderr, "Error: empty file\n");

    else if (status == CSV_ERROR_REPEATING_COLUMNS)
        fprintf(stderr, "Error at line %zu: column names must be unique\n", error_line);

    else if (status == CSV_ERROR_FIELDS_NUMBER)
        fprintf(stderr, "Error at line %zu: the number of fields in the record does not match the number of columns\n", error_line);

    else if (status == CSV_ERROR_EMPTY_LINE_NOT_LAST)
        fprintf(stderr, "Error at line %zu: empty line can only be in the end of the file\n", error_line);

    else if (status == CSV_ERROR_FORMAT)
        fprintf(stderr, "Error at line %zu: invalid CSV format\n", error_line);

    else
        fprintf(stderr, "Error at line %zu\n", error_line);

    return EXIT_ERROR_RUNTIME;
}

ParseCsvStatus parse_csv(FILE *file, ColumnsArray *columns, size_t *error_line)
{
    ParseCsvStatus status;
    Array field;
    *error_line = 1;

    if (initialise(&field, sizeof(char)) != 0)
    {
        return CSV_ERROR_MEMORY;
    }

    if (initialise_columns_arr(columns) != 0)
    {
        free(field.ptr);
        return CSV_ERROR_MEMORY;
    }

    status = parse_header(file, columns, &field);

    switch (status)
    {
    case CSV_OK:
        break;
    case CSV_EOF:
        free(field.ptr);
        post_init(columns);
        return CSV_OK;
    case CSV_EMPTY_LINE:
    case CSV_EMPTY_EOF_LINE:
        free(field.ptr);
        return CSV_ERROR_EMPTY_FILE;
    default:
        free(field.ptr);
        return status;
    }

    for (; (status = parse_record(file, columns, &field)) == CSV_OK; *error_line += 1)
    {
    }
    *error_line += 1;

    if (status == CSV_EMPTY_LINE)
    {
        for (; (status = parse_record(file, columns, &field)) == CSV_EMPTY_LINE;)
        {
        }

        if (status != CSV_EMPTY_EOF_LINE)
        {
            free(field.ptr);
            return CSV_ERROR_EMPTY_LINE_NOT_LAST;
        }
    }

    if (status != CSV_EOF && status != CSV_EMPTY_EOF_LINE)
    {
        free(field.ptr);
        return status;
    }
    post_init(columns);

    free(field.ptr);
    return CSV_OK;
}

ParseCsvStatus parse_header(FILE *file, ColumnsArray *columns, Array *field)
{
    return parse_line(file, columns, field, 1);
}

ParseCsvStatus parse_record(FILE *file, ColumnsArray *columns, Array *field)
{
    return parse_line(file, columns, field, 0);
}

ParseCsvStatus parse_line(FILE *file, ColumnsArray *columns, Array *field, int is_header)
{
    ParseCsvStatus status = CSV_OK;
    size_t fields_count = 0;
    field->elts_number = 0;

    int is_physically_empty = 1;
    char c;
    for (c = getc(file); status == CSV_OK; c = getc(file))
    {
        int is_end_of_line = c == '\n' || c == EOF;

        if (!is_end_of_line && !is_space(c))
        {
            is_physically_empty = 0;
        }

        if (is_physically_empty && is_end_of_line)
        {
            if (c == EOF)
            {
                return CSV_EMPTY_EOF_LINE;
            }
            return CSV_EMPTY_LINE;
        }

        if (is_space(c) && field->elts_number == 0)
            continue;

        if (!is_delimiter(c) && !is_end_of_line)
        {
            if (append_char(field, c) != 0)
            {
                return CSV_ERROR_MEMORY;
            }
            continue;
        }

        // A field is over
        if (append_char(field, '\0') != 0)
        {
            return CSV_ERROR_MEMORY;
        }
        fields_count++;
        if (is_header)
        {
            if (field->elts_number < 2)
            {
                return CSV_ERROR_FORMAT;
            }
            Column column;
            if (trim_right(field) != 0 || initialise_column(&column, field) != 0 || append_column(columns, &column) != 0)
            {
                return CSV_ERROR_MEMORY;
            }
        }
        if (!is_header && fields_count > columns->elts_number)
        {
            return CSV_ERROR_FIELDS_NUMBER;
        }
        if (!is_header)
        {
            update_column(columns, fields_count - 1, field);
        }

        field->elts_number = 0;

        if (is_end_of_line)
        {
            break;
        }
    }

    if (is_header)
    {
        columns->elts_number = fields_count;
        if (has_repeating(columns))
        {
            return CSV_ERROR_REPEATING_COLUMNS;
        }
    }
    if (!is_header && fields_count != columns->elts_number)
        return CSV_ERROR_FIELDS_NUMBER;

    if (c == EOF)
    {
        return CSV_EOF;
    }

    return status;
}
