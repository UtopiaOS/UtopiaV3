

#include <covenant/std.h>

#include "impl.h"

static Int32
cmp(UniversalType a, UniversalType b)
{
    return *(UChar *)a - ((struct fmtverb *)b)->c;
}

Status
c_fmt_install(Rune tgt_char, FormatFunction fn)
{
    struct fmtverb *p;
    struct fmtverb nent;
    Array *ap;
    USize n;
    Byte *s;

    ap = &__fmt_Fmts;
    if (c_dyn_ready(ap, 1, sizeof(nent)))
        return -1;
    
    n = c_arr_len(ap, sizeof(*p));
    s = c_arr_data(ap);
    p = c_std_nbsearch(&tgt_char, s, n, sizeof(*p), &cmp);
    if (p->c == tgt_char) {
        p->fn = fn;
        return 0;
    }

    nent = (struct fmtverb){tgt_char, fn};
	n = (((Byte *)p - s) / sizeof(*p)) + !!n;
    return c_dyn_insert(ap, n, &nent, 1, sizeof(nent));
}