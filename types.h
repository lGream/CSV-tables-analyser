#pragma once

#include <ctype.h>
#include "arrays.h"

typedef enum Type
{
    TYPE_INVALID = 10,
    TYPE_NULL = 11,
    TYPE_INT = 12,
    TYPE_DOUBLE = 13,
} Type;

Type get_type(const Array *string_ptr);