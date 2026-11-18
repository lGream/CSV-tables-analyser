#include "stats.h"
#include "csv_parser.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

void print_stats(const ColumnsArray *columns);

void print_column(const ColumnsArray *columns, size_t i);

ExitCode run_stats(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Error: missing <csv-file> for stats command\n");
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
    ExitCode code = run_csv_parser(file, &columns);
    fclose(file);

    if (code != EXIT_OK)
    {
        free_columns(&columns);
        return code;
    }

    print_stats(&columns);
    free_columns(&columns);

    return EXIT_OK;
}

void print_stats(const ColumnsArray *columns)
{
    printf("rows %zu\n", get_column_ptr(columns, 0)->count_total);
    printf("columns %zu\n\n", columns->elts_number);
    for (size_t i = 0; i < columns->elts_number; i++)
    {
        print_column(columns, i);
    }
}

void print_column(const ColumnsArray *columns, size_t i)
{
    Column *column = get_column_ptr(columns, i);
    printf("column %zu\n", i);
    printf("name \"%s\"\n", column->name);
    printf("type %s\n", column->type_str);
    printf("nulls %zu\n", column->nulls);
    if (column->type == TYPE_INT)
    {
        printf("min %lld\n", column->int_min);
        printf("max %lld\n", column->int_max);
    }
    else
    {
        printf("min %.6Lf\n", column->min);
        printf("max %.6Lf\n", column->max);
    }
    printf("mean %Lf\n", column->mean);
    printf("positive %zu\n", column->positive);
    printf("negative %zu\n", column->negative);
    if (column->type == TYPE_INT)
    {
        printf("even %zu\n\n", column->even);
    }
    else
    {
        printf("integer %zu\n\n", column->integer);
    }
}
