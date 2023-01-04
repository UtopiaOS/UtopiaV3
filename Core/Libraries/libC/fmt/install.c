

#include <covenant/std.h>

#include "impl.h"

static i32
cmp(void *a, void *b)
{
    return *(uchar *)a - ((struct fmtverb *)b)->c;
}

ctype_status
c_fmt_install(ctype_rune tgt_char, ctype_fmtfn fn)
{
    struct fmtverb *p;
    struct fmtverb nent;
    ctype_arr *ap;
    usize n;
    uchar *s;

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
	n = (((uchar *)p - s) / sizeof(*p)) + !!n;
    return c_dyn_insert(ap, n, &nent, 1, sizeof(nent));
}