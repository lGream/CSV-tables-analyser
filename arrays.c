#include "arrays.h"
#include "chars.h"
#include <string.h>
#include <stdlib.h>


int initialise(Array *array, size_t elt_size)
{
    void *ptr = malloc(elt_size);
    if (array == NULL || ptr == NULL)
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

int append_char(Array *array, char c)
{
    return append(array, &c);
}

int append_string(Array *array, const char *string)
{
    return append(array, &string);
}

int trim_right(Array *string)
{
    char *string_ptr = string->ptr;
    void *new_ptr;
    size_t new_elts_number = string->elts_number;
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
