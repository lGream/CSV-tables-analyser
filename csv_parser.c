#include "columns.h"
#include "csv_parser.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

ExitCode parse(FILE *file, ColumnsArray *columns)
{
    size_t error_line;
    ParseCsvStatus status = parse_csv(file, columns, &error_line);
    if (status == CSV_OK)
    {
        return EXIT_OK;
    }
    if (status == CSV_ERROR_FORMAT)
    {
        fprintf(stderr, "Error at line %llu: invalid CSV format\n", error_line);
    }
    if (status == CSV_ERROR_MEMORY)
    {
        fprintf(stderr, "Error at line %llu\n", error_line);
    }
    return EXIT_ERROR_RUNTIME;
}

ParseCsvStatus parse_line(FILE *file, ColumnsArray *columns, int is_header)
{
    ParseCsvStatus status = CSV_OK;
    Array field;
    initialise(&field, sizeof(char));
    size_t fields_count = 0;

    int is_physically_empty = 1;
    char c;
    for (c = getc(file); status == CSV_OK; c = getc(file))
    {
        int is_end_of_line = c == '\n' || c == EOF;

        if (!is_end_of_line && !is_space(c))
        {
            is_physically_empty = 0;
        }

        if (is_space(c) && field.elts_number == 0)
            continue;

        if (!is_delimiter(c) && !is_end_of_line)
        {
            if (append_char(&field, c) != 0)
            {
                return CSV_ERROR_MEMORY;
            }
            continue;
        }

        // Поле закончилось
        if (append_char(&field, '\0') != 0)
        {
            return CSV_ERROR_MEMORY;
        }
        fields_count++;
        if (is_header)
        {
            if (field.elts_number < 2)
            {
                return CSV_ERROR_FORMAT;
            }
            Column column;
            if (initialise_column(&column, &field) != 0 || append_column(columns, &column) != 0)
            {
                return CSV_ERROR_MEMORY;
            }
        }
        if (!is_header && fields_count > columns->elts_number)
        {
            return CSV_ERROR_FORMAT;
        }
        if (!is_header)
        {
            update_column(columns, fields_count - 1, &field);
        }

        clear(&field);

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
            return CSV_ERROR_FORMAT;
        }
    }
    if (!is_header && fields_count != columns->elts_number)
        return CSV_ERROR_FORMAT;

    if (is_physically_empty && c == EOF)
    {
        return CSV_EMPTY_EOF_LINE;
    }
    if (c == EOF)
    {
        return CSV_EOF;
    }
    if (is_physically_empty)
    {
        return CSV_EMPTY_LINE;
    }
    return status;
}

ParseCsvStatus parse_header(FILE *file, ColumnsArray *columns)
{
    return parse_line(file, columns, 1);
}

ParseCsvStatus parse_record(FILE *file, ColumnsArray *columns)
{
    return parse_line(file, columns, 0);
}

ParseCsvStatus parse_csv(FILE *file, ColumnsArray *columns, size_t *error_line)
{
    *error_line = 1;
    ParseCsvStatus status;
    if (initialise_columns_arr(columns) != 0)
    {
        return CSV_ERROR_MEMORY;
    }
    status = parse_header(file, columns);

    if (status != CSV_OK && status != CSV_EOF)
    {
        return status;
    }

    for (; (status = parse_record(file, columns)) == CSV_OK; *error_line += 1)
    {
    }
    *error_line += 1;

    if (status == CSV_EMPTY_LINE)
    {
        for (; (status = parse_record(file, columns)) == CSV_EMPTY_LINE;)
        {
        }
    }
    if (status != CSV_EOF && status != CSV_EMPTY_EOF_LINE)
    {
        return status;
    }

    post_init(columns);

    return CSV_OK;
}
