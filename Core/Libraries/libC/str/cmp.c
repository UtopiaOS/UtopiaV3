
#include <covenant/std.h>

i32
c_str_cmp(char *v1, usize n, char *v2)
{
    uchar *s1, *s2;
    i32 diff;

    s1 = (uchar *)v1;
    s2 = (uchar *)v2;

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