#pragma once
#include "arrays.h"

static const long long LOWER_INT_BOUND = -9223372036854775807LL - 1LL;
static const long long UPPER_INT_BOUND = 9223372036854775807LL;

typedef enum
{
    TYPE_INVALID = 10,
    TYPE_NULL = 11,
    TYPE_INT = 12,
    TYPE_DOUBLE = 13,
} Type;

Type get_type(const Array *string_ptr);