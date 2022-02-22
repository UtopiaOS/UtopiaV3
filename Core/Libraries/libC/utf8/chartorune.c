
#include <covenant/std.h>

i32
c_utf8_chartorune(ctype_rune *p, char *s)
{
    return c_utf8_charntorune(p, s, C_UTFMAX);
}