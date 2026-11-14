#include "exit_codes.h"
#include "help.h"
#include "schema.h"
#include "stats.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error: No command specified. Use --help\n");
        return EXIT_ERROR_ARGUMENTS;
    }

    if (strcmp(argv[1], "--help") == 0)
    {
        return run_help(argc);
    }
    if (strcmp(argv[1], "schema") == 0)
    {
        return run_schema(argc, argv);
    }
    if (strcmp(argv[1], "stats") == 0)
    {
        return run_stats(argc, argv);
    }

    fprintf(stderr, "Error: unknown command '%s'\n", argv[1]);
    return EXIT_ERROR_ARGUMENTS;
}