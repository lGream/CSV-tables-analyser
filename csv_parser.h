#pragma once
#include "exit_codes.h"
#include "columns.h"

ExitCode run_csv_parser(FILE *file, ColumnsArray *columns);
