
#include <covenant/std.h>

Status
__fmt_trycat(Format *p, char *s, USize n)
{
    if (!n)
        return StatusOk;
    
    if (p->fn(p, s, n) < 0)
        return StatusErr;
    
    p->nfmt += n;
    return StatusOk;
}


static Status
pad(Format *p, USize n)
{
    Int32 w;

    for (w = p->width - n; w > 0; --w) {
        if(__fmt_trycat(p, " ", 1)  < 0)
            return StatusErr;
    }

    return StatusOk;
}

Status
c_fmt_nput(Format *p, char *s, USize n)
{
    if (!s) {
        s = "<nil>";
        n = 5;
    }

    if ((p->flags & C_FMTPREC) && n > (USize)p->prec)
        n = p->prec;
    
    if (!(p->flags & C_FMTLEFT) && pad(p, n) < 0)
        return StatusErr;
    
    if (__fmt_trycat(p, s, n) < 0)
        return StatusErr;

    if ((p->flags & C_FMTLEFT) && pad(p, n) < 0)
        return StatusErr;
    
    return StatusOk;
}