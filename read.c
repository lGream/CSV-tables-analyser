#include <stdio.h>
#include "file_working.h"

int main(void)
{
    FILE *file = fopen("test1.csv", "r");
    if (file == NULL)
    {
        return 1;
    }

    int columns_count;
    int is_header;
    char *field = malloc(sizeof(char));
    struct Column *columns;

    read_line(file, &field, &columns_count, &columns, 1);
    columns = malloc(columns_count * sizeof(struct Column));
    if (initialise(&columns, columns_count, fields) != 0)
    {
        return 1;
    }
    
    free(fields);
    fields = malloc(sizeof(char *));

    int code;

    while ((code = read_line(file, &fields, &columns_count, &columns, 0)) == 0)
    {
        free(fields);
        fields = malloc(sizeof(char *));
    }

    if (code != EOF)
    {
        return code;
    }

    for (int i = 0; i < columns_count; i++)
    {
        struct Column column = columns[i];
        columns[i].mean = column.sum / column.count;
        column = columns[i];
        printf("\"%s\"", column.name);
    }

    free(columns);

    return 0;
}