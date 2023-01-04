
#include <covenant/std.h>

Int32
c_utf8_chartorune(Rune *p, char *s)
{
    return c_utf8_charntorune(p, s, C_UTFMAX);
}