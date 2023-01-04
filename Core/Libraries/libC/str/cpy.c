#include <covenant/std.h>

char *
c_str_cpy(char *dest, USize n, char *src)
{
    for(;n && (*dest = *src); --n, ++dest, ++src);
    return dest;
}