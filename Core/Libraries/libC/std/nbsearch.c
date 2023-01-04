#include <covenant/std.h>

UniversalType
c_std_nbsearch(UniversalType k, UniversalType buf, USize m, USize n, CompareFunction fn)
{
    Int32 r;
    Byte *p, *s;

    r = 0;
    p = s = (Byte *)buf;

    while (m) {
        p = s + (m >> 1) *n;
        if (!(r = fn(k,p))) {
            return p;
        } else if (r > 0) {
            s = p + n;
            m--;
        }
        m >>= 1;
    }
    if ( r > 0) {
        return p;
    }
    return (p == buf) ? p : p - n;
}