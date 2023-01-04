
#include <covenant/std.h>

USize
c_str_len(char *s, USize n)
{
    char *p;

    if (!(p = c_mem_chr(s, n, 0)))
        return n;
    
    return p - s;
}
