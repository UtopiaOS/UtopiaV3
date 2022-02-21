

#include <covenant/std.h>

void
c_fmt_init(ctype_fmt *p, void *farg, ctype_arr *array, ctype_fmtopfn fn)
{
    p->array = array;
    p->fn = fn;
    p->farg = farg;
    p->nfmt = 0;
    p->flags = 0;
    p->width = 0;
    p->prec = 0;
}