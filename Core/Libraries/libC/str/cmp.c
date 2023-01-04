
#include <covenant/std.h>

Int32
c_str_cmp(char *v1, USize n, char *v2)
{
    UChar *s1, *s2;
    Int32 diff;

    s1 = (UChar *)v1;
    s2 = (UChar *)v2;

    for (;n;--n) {
        if ((diff = *s1 - *s2))
            return diff;
        if (!*s1)
            return 0;
        s1++;
        s2++;
    }
    return 0;
}