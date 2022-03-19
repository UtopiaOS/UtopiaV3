#include <covenant/std.h>

char *
c_str_pncpy(char *restrict s1, const char *restrict s2, size n)
{
    while (n-- && (*s1++ = *s2++));
    return s1;
}