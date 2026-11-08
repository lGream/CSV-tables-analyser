#include <stdlib.h>
#include <stdio.h>
#include "array_stuff.h"

int main(void)
{
    int lines_count = 0;
    int lines_capacity = 1;
    char **lines = malloc(lines_capacity * sizeof(char *));

    int count = 0;
    int capacity = 1;
    char *line = malloc(sizeof(char));

    FILE *file = fopen("test1.csv", "r");
    if (file == NULL)
        return 1;
    char c;
    while (1)
    {
        c = getc(file);
        if (c == '\n' || c == EOF)
        {

            line[count] = '\0';
            lines_count++;
            expand_array_of_strings(&lines, &lines_capacity, lines_count);
            lines[lines_count - 1] = line;
            reset_string(&line, &capacity, &count);
            if (c == EOF)
                break;
            continue;
        }
        count++;
        expand_string(&line, &capacity, count);
        line[count - 1] = c;
    }

    for (int i = 0; i < lines_count; i++)
    {
        printf("%s\n", lines[i]);
    }

}
