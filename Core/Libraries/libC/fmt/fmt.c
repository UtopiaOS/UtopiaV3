
#include <covenant/std.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>

#include "impl.h"

static Int32
cmp(UniversalType a, UniversalType b)
{
    return *(Byte *)a - ((struct fmtverb *)b)->c;
}

static Status
fmtfmt(Format *fmt, Byte *str)
{
    struct fmtverb *p;
    Array *ap;
    Byte *sp;

    ap = &__fmt_Fmts;
    fmt->r = *str;
    if ((p = c_std_bsearch(str, c_arr_data(ap), c_arr_len(ap, sizeof(*p)), sizeof(*p), &cmp)))  {
        return (p->fn)(fmt);
    }

    sp = (UniversalType)(UIntPtr)__fmt_VFmts;
    if ((p = c_std_bsearch(str, sp, VFMTLEN, sizeof(*p), &cmp))) {
        if (!c_fmt_install(p->c, p->fn))
            return (p->fn)(fmt);
    }

    errno = C_EINVAL;
    return StatusErr;
}

static Size
fmtflag(Format *f, Byte *s)
{
    Status ret;
    Size nmft;
    Int32 i;

    f->flags = 0;
    f->width = 0;
    f->prec = 0;

    nmft = 0;

    for (;;) {
        nmft++;
        if (!(f->r = *s++))
            return -1;
        
        switch(f->r) {
            case '.':
                f->flags = C_FMTWIDTH | C_FMTPREC;
                continue;
            case '0':
                if (!(f->flags & C_FMTWIDTH)) {
                    f->flags |= C_FMTZERO;
                    continue;
                }
            /* FALLTHROUGHT */
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                i = 0;
                while (*s >= '0' && *s <= '9') {
                    i = i * 10 + *s - '0';
                    s++;
                    nmft++;
                }
                s--;
                nmft--;
                numflag:
                    if (f->flags & C_FMTWIDTH) {
                        f->flags |= C_FMTPREC;
                        f->prec = i;
                    } else {
                        f->flags |= C_FMTWIDTH;
                        f->width = i;
                    }
                    continue;
            case '*':
                if ((i = va_arg(f->args, i32)) < 0) {
                    if (f->flags & C_FMTPREC) {
                        f->flags &= ~C_FMTPREC;
                        f->prec = 0;
                        continue;
                    }
                    i = -i;
                    f->flags |= C_FMTLEFT;
                }
                goto numflag;
        }
        if ((ret = fmtfmt(f, s)) < 0)
            return -1;
        if (!ret)
            return nmft;
    }
}

Size
c_fmt_fmt(Format *p, char *fmt)
{
    Size n;
    USize nfmt;
    Int32 ch;
    Byte *s;

    s = (Byte *)fmt;
    nfmt = p->nfmt;
    while (*s) {
        n = 0;
        for (;;) {
            ch = *(s + n);
            if (ch == '%' || ch == '\0')
                break;
            n++;
        }
        __fmt_trycat(p, (char *)s, n);
        s += n;
        if (ch == '%') {
            if ((n = fmtflag(p, s)) < 0)
                return -1;
            s += n + 1;
        }
    }
    return p->nfmt - nfmt;
}