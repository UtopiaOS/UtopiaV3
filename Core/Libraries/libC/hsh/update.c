/*
 *
 * Designed by: Eduardo Francisco.
*/

#include <covenant/std.h>

typedef void (*hfunc)(ctype_hst *, char*);

void
c_hsh_update(hfunc f, int b, ctype_hst *p, char *data, usize n)
{
    usize len;

    while (n) {
        if (!p->curlen && n >= (u32)b) {
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
            if (p->curlen == (u32)b) {
                f(p, data);
                p->len += b << 3;
                p->curlen = 0;
            }
        }
    }
}