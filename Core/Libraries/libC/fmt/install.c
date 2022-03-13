

#include <covenant/std.h>

#include "impl.h"

static i32
cmp(const void *a, const void *b)
{
    return (*(char *)a - ((struct fmtverb *)b)->c);
}

ctype_status
c_fmt_install(i32 tgt_char, ctype_fmtfn fn)
{
    struct fmtverb *l, *p;
    struct fmtverb nf;
    usize n;

    nf = (struct fmtverb){tgt_char, fn};
    if (!(n = c_arr_bytes(&__fmt_Fmts)))
        return c_dyn_cat(&__fmt_Fmts, &nf, 1, sizeof(nf));
    if ((l = c_arr_data(&__fmt_Fmts))->c > tgt_char)
        return c_dyn_idxcat(&__fmt_Fmts, 0, &nf, 1, sizeof(nf));
    n /= sizeof(*p);
    p = c_std_nbsearch(&tgt_char, l, n, sizeof(*l), &cmp);
    if (p->c == tgt_char) {
        p->fn = fn;
        return 0;
    }
    return c_dyn_idxcat(&__fmt_Fmts, (p + 1) - l, &nf, 1, sizeof(nf));
}