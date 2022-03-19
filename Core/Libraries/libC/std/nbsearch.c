#include <covenant/std.h>

void *
c_std_nbsearch(void *k, void *buf, usize m, usize n, ctype_cmpfn fn)
{
    i32 r;
    uchar *p, *s;

    r = 0;
    p = s = (uchar *)buf;

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
    return r > 0 ? p : p - n;
}