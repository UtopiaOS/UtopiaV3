
#include <covenant/std.h>

#include "impl.h"

#define __getbase(a) \
((a) == 'p' || (a | 32) == 'x' ? 16 : ((a) == 'o') ? 8 : ((a) == 'b') ? 2 : 10)

static Status Vchar(Format *);
static Status Verr(Format *);
static Status Vflag(Format *);
static Status Vint(Format *);
static Status Vperc(Format *);
static Status Vstr(Format *);

Array __fmt_Fmts;

struct fmtverb __fmt_VFmts[] = {
    { ' ', Vflag },
    { '#', Vflag },
    { '%', Vperc },
    { '+', Vflag },
    { ',', Vflag },
    { '-', Vflag },
    { 'X', Vint },
    { 'b', Vint },
    { 'c', Vchar },
    { 'd', Vint },
    { 'h', Vflag },
    { 'l', Vflag },
    { 'o', Vint },
    { 'p', Vint },
    { 'r', Verr },
    { 's', Vstr },
    { 'u', Vflag },
    { 'x', Vint },
    { 'z', Vflag }, 
};

static Status
Vchar(Format *p)
{
    Rune run;
    char buf[8];

    run = va_arg(p->args, Rune);
    c_mem_cpy(buf, sizeof(run), &run);
    return c_fmt_nput(p,buf, c_utf8_chartorune(&run, buf));
}

static Status
Verr(Format *p)
{
    // TODO: Make it part of status?
    ctype_error errnum;
    char buf[C_ERRSIZ];

    // TODO: We might to make "error" part of status
    if ((errnum = va_arg(p->args, ctype_error)) == C_ECSTM)
        c_std_errstr(buf, sizeof(buf));
    else
        c_std_strerror(errnum, buf, sizeof(buf));

    return c_fmt_put(p, buf);
}

static Status
Vflag(Format *p)
{
    switch(p->r) {
        case ',':
            p->flags |= C_FMTCOMMA;
            break;
        case '-':
            p->flags |= C_FMTLEFT;
            break;
        case '+':
            p->flags |= C_FMTSIGN;
            break;
        case '#':
            p->flags |= C_FMTSHARP;
            break;
        case ' ':
            p->flags |= C_FMTSPACE;
            break;
        case 'u':
            p->flags |= C_FMTUNSIGNED;
            break;
        case 'h':
            if (p->flags & C_FMTSHORT)
                p->flags |= C_FMTBYTE;
            p->flags |= C_FMTSHORT;
            break;
        case 'l':
            if (p->flags & C_FMTLONG)
                p->flags |= C_FMTVLONG;
            p->flags |= C_FMTLONG;
            break;
        case 'z':
            p->flags |= C_FMTLONG;
            if (sizeof(UIntPtr) == sizeof(UVLong))
                p->flags |= C_FMTVLONG;
            break;
    }
    return StatusOk;
}

static Status
Vint(Format *p)
{
    UVLong l;
    Int32 b, d, i, j, u, neg;
    char buf[64];
    
    u = p->flags & C_FMTUNSIGNED;
    neg = 0;

	if (p->flags & C_FMTVLONG) {
		l = va_arg(p->args, UVLong);
		if (!u && (VLong)l < 0) l = (++neg, -(VLong)l);
	} else if (p->flags & C_FMTLONG) {
		l = va_arg(p->args, ULong);
		if (!u && (long)l < 0) l = (++neg, -(long)l);
	} else {
		l = va_arg(p->args, UInt32);
		if (!u && (Int32)l < 0) l = (++neg, -(Int32)l);
 	}


    b = __getbase(p->r);
    u = (p->r == 'X') ? 'A' : 'a';
    i = sizeof(buf) - 1;
    j = 0;

    if (!l)
        buf[--i] = '0';
    
    for (; l; ++j) {
        d = (l % b);
        if ((p->flags & C_FMTCOMMA) && j % 4 == 3) {
            buf[--i] = ',';
            ++j;
        }
        buf[--i] = (d < 10) ? d + '0' : u + d - 10;
        l /= b;
    }

    if ((p->flags & C_FMTZERO) && !(p->flags & (C_FMTLEFT | C_FMTPREC))) {
        p->width -= sizeof(buf) - i;
        for (; p->width >= 0; --p->width) {
            buf[--i] = '0';
        }
        p->width = 0;
    }

    if (p->flags & C_FMTSHARP) {
        if (b == 16)
            buf[--i] = u + 23;
        if (b == 16 || b == 8)
            buf[--i] = '0';
    }

    if (neg)
        buf[--i] = '-';
    else if (p->flags & C_FMTSIGN)
        buf[--i] = '+';
    else if (p->flags & C_FMTSPACE)
        buf[--i] = ' ';
    
    return c_fmt_nput(p, buf + i, sizeof(buf) - (i + 1));
}

static Status
Vperc(Format *p)
{
    p->prec = 1;
    return c_fmt_nput(p, "%", 1);
}

static Status
Vstr(Format *p)
{
    char *s;

    s = va_arg(p->args, char *);
    return c_fmt_put(p, s);
}