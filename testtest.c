#include "array_stuff.h"
#include <stdio.h>

int main(void)
{
    char *buffer;
    char *string = "abc";
    if (wrap_in_quotes(string, &buffer) != 0)
    {
        fprintf(stderr, "Ошибка\n");
        return 1;
    }
    printf("%s\n", buffer);
    return 0;
}