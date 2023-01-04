

#include <covenant/std.h>

Status
c_fmt_put(Format *p, char *s)
{
    USize len = 0;
    if (s) {
        len = c_str_len(s, -1);
    }
    return c_fmt_nput(p, s, len);
}