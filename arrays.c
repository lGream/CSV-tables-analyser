#include "arrays.h"
#include <string.h>
#include <stdlib.h>

int is_space(char c)
{
    return c == ' ' || c == '\t';
}

int is_delimiter(char c)
{
    return c == ',';
}

typedef struct Array
{
    void *ptr;
    size_t capacity;
    size_t elts_number;
    size_t elt_size;
} Array;

int initialise(Array *array, size_t elt_size)
{
    assert(array != NULL);
    void *ptr = malloc(elt_size);
    if (ptr == NULL)
    {
        return 1;
    }
    array->ptr = ptr;
    array->capacity = 1;
    array->elts_number = 0;
    array->elt_size = elt_size;

    return 0;
}

int expand(Array *array, size_t new_elts_number)
{
    if (array == NULL)
    {
        return 1;
    }

    size_t new_capacity = array->capacity;

    if (array->capacity < 1)
    {
        new_capacity = 1;
    }
    if (array->capacity > new_elts_number)
    {
        return 0;
    }

    while (new_capacity <= new_elts_number)
    {
        new_capacity *= 2;
    }
    void *new_ptr = realloc(array->ptr, new_capacity * array->elt_size);

    if (new_ptr == NULL)
    {
        return 1;
    }

    array->ptr = new_ptr;
    array->capacity = new_capacity;

    return 0;
}

int append(Array *array, void *elt)
{
    if (array == NULL)
    {
        return 1;
    }

    size_t new_elts_number = array->elts_number + 1;

    if (expand(array, new_elts_number) != 0)
    {
        return 1;
    }

    char *array_ptr = array->ptr;
    char *destination = array_ptr + array->elts_number * array->elt_size;
    memcpy(destination, elt, array->elt_size);

    array->elts_number = new_elts_number;

    return 0;
}

int clear(Array *array)
{
    if (array == NULL || array->ptr == NULL)
    {
        return 1;
    }

    array->elts_number = 0;

    void *ptr = malloc(array->capacity * array->elt_size);
    if (ptr == NULL)
    {
        return 1;
    }

    array->ptr = ptr;
    return 0;
}

int append_char(Array *array, char c)
{
    return append(array, &c);
}

int append_string(Array *array, const char *string)
{
    return append(array, &string);
}

int wrap_in_quotes(Array *string)
{

    if (string == NULL || string->ptr == NULL)
    {
        return 1;
    }

    size_t len = string->elts_number - 1;

    char *buffer = malloc((len + 3) * sizeof(char));

    if (buffer == NULL)
    {
        return 1;
    }

    buffer[0] = '"';
    buffer[len + 1] = '"';
    buffer[len + 2] = '\0';
    memcpy(buffer + 1, string->ptr, len);

    void *new_ptr = realloc(string->ptr, (len + 3) * sizeof(char));

    if (new_ptr == NULL)
    {
        return 1;
    }

    memcpy(new_ptr, buffer, len + 3);
    free(buffer);
    string->ptr = new_ptr;

    return 0;
}

int trim_right(Array *string)
{
    char *string_ptr = string->ptr;
    void *new_ptr;
    size_t new_elts_number;
    for (size_t len = string->elts_number; len > 0; len--)
    {
        char c = string_ptr[len - 2];
        if (is_space(c))
        {
            continue;
        }
        new_elts_number = len;
        string_ptr[len - 1] = '\0';
        break;
    }
    if (new_elts_number == string->elts_number)
    {
        return 0;
    }
    new_ptr = realloc(string->ptr, new_elts_number);

    if (new_ptr == NULL)
    {
        return 1;
    }

    string->ptr = new_ptr;
    string->elts_number = new_elts_number;
    return 0;
}
