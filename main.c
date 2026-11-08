#include <stdio.h>
#include "types_determination.h"

int main(void)
{
    char *string = "-9223372036854775809";
    printf("%llu\n", sizeof(long double));
}