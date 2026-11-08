#pragma once
#include <stdlib.h>
#pragma once
#include <string.h>

int expand_array(void **array, int *capacity, int count, size_t element_size)
{
    if (array == NULL || *array == NULL || capacity == NULL)
    {
        return 1;
    }

    if (*capacity < 1)
    {
        *capacity = 1;
    }

    if (count + 1 <= *capacity)
    {
        return 0;
    }

    int new_capacity = *capacity;

    while (count > new_capacity)
    {
        new_capacity *= 2;
    }

    char *new_memory = realloc(*array, (new_capacity * element_size));

    if (new_memory == NULL)
    {
        return 1;
    }

    *array = new_memory;
    *capacity = new_capacity;

    return 0;
}

int expand_string(char **string, int *capacity, int count)
{
    return expand_array((void **)string, capacity, count + 1, sizeof(char)); // count + 1, чтобы было дополнительное место для '\0'
}

int expand_array_of_strings(char ***strings, int *capacity, int count)
{
    return expand_array((void **)strings, capacity, count, sizeof(char *));
}

int reset_array(void **array, int *capacity, int *count, size_t element_size)
{
    if (array == NULL || capacity == NULL || count == NULL)
    {
        return 1;
    }
    *count = 0;
    *capacity = 1;
    char *new_memory = malloc(*capacity * element_size);
    if (new_memory == NULL)
    {
        return 1;
    }

    *array = new_memory;

    return 0;
}

int reset_string(char **string, int *capacity, int *count)
{
    int code = reset_array((void **)string, capacity, count, sizeof(char));
    if (code != 0)
    {
        return code;
    }
    *(string)[0] = '\0';
    return code;
}

int reset_array_of_strings(char ***strings, int *capacity, int *count)
{
    return reset_array((void **)strings, capacity, count, sizeof(char *));
}

int has_repeats(char **array, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j)
                continue;
            if (strcmp(array[i], array[j]) != 0)
                continue;
            return 1;
        }
    }
    return 0;
}

int wrap_in_quotes(char *string, char **buffer)
{
    if (buffer == NULL || *buffer == NULL)
    {
        return 1;
    }

    size_t len = strlen(string) + 3;

    *buffer = malloc(len * sizeof(char));

    (*buffer)[0] = '"';
    (*buffer)[len - 2] = '"';
    (*buffer)[len - 1] = '\0';

    for (int i = 1; i < len - 2; i++)
    {
        (*buffer)[i] = string[i - 1];
    }

    return 0;
}
