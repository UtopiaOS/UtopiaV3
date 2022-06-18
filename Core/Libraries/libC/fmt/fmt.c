
#include <covenant/std.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>

#include "impl.h"

static i32
cmp(void *a, void *b)
{
    return (*(uchar *)a - ((struct fmtverb *)b)->c);
}

static ctype_status
fmtfmt(ctype_fmt *fn, uchar *str)
{
    struct fmtverb *p;
    ctype_arr *ap;
    uchar *sp;

    ap = &__fmt_Fmts;

    if ((p = c_std_bsearch(str, c_arr_data(ap), c_arr_len(ap, sizeof(*p)), sizeof(*p), &cmp)))  {
        return (p->fn)(fn);
    }

    sp = (void*)(uintptr)__fmt_VFmts;
    if ((p = c_std_bsearch(str, sp, VFMTLEN, sizeof(*p), &cmp))) {
        if (!c_fmt_install(p->c, p->fn))
            return (p->fn)(fn);
    }

    errno = C_EINVAL;
    return -1;
}

static size
fmtflag(ctype_fmt *f, uchar *s)
{
    ctype_status ret;
    size nmft;
    i32 i;

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

size
c_fmt_fmt(ctype_fmt *p, char *fmt)
{
    size n;
    usize nfmt;
    i32 ch;
    uchar *s;

    s = (uchar *)fmt;
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