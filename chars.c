#include "chars.h"

int is_space(char c)
{
    return c == ' ' || c == '\t';
}

int is_delimiter(char c)
{
    return c == ',';
}
