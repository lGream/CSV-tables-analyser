#include "help.h"
#include <stdio.h>

ExitCode run_help(int argc)
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: --help does not take additional arguments\n");
        return EXIT_ERROR_ARGUMENTS;
    }

    printf("Available commands:\n");
    printf("csvstat --help\n");
    printf("csvstat schema <csv-file> [options]\n");
    printf("csvstat stats <csv-file> [options]\n");

    return EXIT_OK;
}