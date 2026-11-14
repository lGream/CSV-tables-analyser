#include "types.h"
#include <ctype.h>
#include <stdlib.h>

const long long LOWER_INT_BOUND = -9223372036854775807LL - 1LL;
const long long UPPER_INT_BOUND = 9223372036854775807LL;

Type get_type(const Array *string_ptr)
{
    if (string_ptr == NULL || string_ptr->ptr == NULL)
    {
        return TYPE_INVALID;
    }

    if (string_ptr->elts_number == 1)
    {
        return TYPE_NULL;
    }
    
    char *string = string_ptr->ptr;

    int i = 0;
    int is_negative = 0;

    if (string[i] == '-')
    {
        is_negative = 1;
        i++;
    }
    else if (string[i] == '+')
    {
        i++;
    }

    long long number = 0;

    while (string[i] != '\0')
    {
        char c = string[i];

        if (c == '.')
        {
            return TYPE_DOUBLE;
        }

        if ('0' <= c && c <= '9')
        {
            int digit = c - '0';

            if (is_negative)
            {
                if (number < (LOWER_INT_BOUND + digit) / 10)
                {
                    return TYPE_DOUBLE;
                }
                number = number * 10 - digit;
            }
            else
            {
                if (number > (UPPER_INT_BOUND - digit) / 10)
                {
                    return TYPE_DOUBLE;
                }
                number = number * 10 + digit;
            }
        }
        i++;
    }

    return TYPE_INT;
}
