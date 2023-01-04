

#include <covenant/std.h>

Void
c_fmt_init(Format *p, UniversalType farg, FormatOperationFunction fn)
{
    p->fn = fn;
    p->farg = farg;
    p->nfmt = 0;
    p->flags = 0;
    p->width = 0;
    p->prec = 0;
}