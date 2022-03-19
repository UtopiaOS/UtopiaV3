#include <covenant/std.h>

char *
c_str_ncpy(char *restrict s1, char *restrict s2, size n)
{
    (void)c_str_pncpy(s1, s2, n);
    return s1;
}