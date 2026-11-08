#pragma once
#include "ctype.h"

const long long LOWER_INT_BOUND = -9223372036854775807LL - 1LL;
const long long UPPER_INT_BOUND = 9223372036854775807LL;

typedef enum
{
    INT = 10,
    DOUBLE = 11,
    INVALID = 12,
} ValueType;

ValueType type(char *string)
{
    if (string == NULL || *string == '\0')
    {
        return INVALID;
    }

    int i = 0;
    int is_negative = 0;

    // 1. Обрабатываем знак числа
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

    // 2. Идем по строке и собираем число
    while (string[i] != '\0')
    {
        char c = string[i];

        // Если встретили точку — это гарантированно дробное число
        if (c == '.')
        {
            return DOUBLE;
        }

        if (c >= '0' && c <= '9')
        {
            int digit = c - '0';

            // Проверка на переполнение ДО умножения и сложения
            if (is_negative)
            {
                if (number < (LOWER_INT_BOUND + digit) / 10)
                {
                    return DOUBLE; // Переполнение вниз
                }
                number = number * 10 - digit;
            }
            else
            {
                if (number > (UPPER_INT_BOUND - digit) / 10)
                {
                    return DOUBLE; // Переполнение вверх
                }
                number = number * 10 + digit;
            }
        }
        i++;
    }

    return INT;
}
