#include "schema.h"
#include "csv_parser.h"
#include <stdio.h>

ExitCode run_schema(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Error: missing <csv-file> for schema command\n");
        return EXIT_ERROR_ARGUMENTS;
    }
    if (argc > 3)
    {
        fprintf(stderr, "Error: unknown argument '%s'\n", argv[3]);
        return EXIT_ERROR_ARGUMENTS;
    }

    const char *filename = argv[2];
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        return EXIT_ERROR_RUNTIME;
    }

    ColumnsArray columns;
    ExitCode code = parse(file, &columns);
    if (code != EXIT_OK)
    {
        return code;
    }

    printf("rows %llu\n", get_column_ptr(&columns, 0)->count);
    printf("columns %llu\n", columns.elts_number);
    for (size_t i = 0; i < columns.elts_number; i++)
    {
        Column *column = get_column_ptr(&columns, i);
        printf("%llu %s %s", i, column->name, column->type_str);
    }

    return EXIT_OK;
}