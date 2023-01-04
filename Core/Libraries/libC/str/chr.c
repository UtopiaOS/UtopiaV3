#include <covenant/std.h>

char *
c_str_chr(char *v, USize n, Int32 c)
{
    UChar *s;

    s = (UChar *)v;
    for (; n && *s; --n) {
        if (*s == c)
            return (char *)s;
        ++s;
    }
    return nil;
}