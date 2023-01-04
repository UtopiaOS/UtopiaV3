/*
 *
 * Designed by: Eduardo Francisco.
*/

#include <covenant/std.h>
#include <covenant/hsh.h>

void
c_hsh_update(hfunc f, int b, ctype_hst *p, char *data, USize n)
{
    USize len;

    while (n) {
        if (!p->curlen && n >= (UInt32)b) {
            f(p, data);
            p->len += b << 3;
            data += b;
            n -= b;
        } else {
            len = C_MIN(n, b - p->curlen);
            c_mem_cpy(p->buf + p->curlen, len, data);
            p->curlen += len;
            data += len;
            n -= len;
            if (p->curlen == (UInt32)b) {
                f(p, data);
                p->len += b << 3;
                p->curlen = 0;
            }
        }
    }
}