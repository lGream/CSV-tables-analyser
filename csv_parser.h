#pragma once

#include "main.h"
#include "columns.h"
#include <stdio.h>

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
} ParseCsvStatus;

ExitCode parse(FILE *file, ColumnsArray *columns);

ParseCsvStatus parse_line(FILE *file, ColumnsArray *columns, int is_header);


ParseCsvStatus parse_header(FILE *file, ColumnsArray *columns);

ParseCsvStatus parse_record(FILE *file, ColumnsArray *columns);

ParseCsvStatus parse_csv(FILE *file, ColumnsArray *columns, size_t *error_line);
