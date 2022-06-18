
#include <covenant/std.h>

#include "impl.h"

#define getbase(a) \
((a) == 'p' || (a | 32) == 'x' ? 16 : ((a) == 'o') ? 8 : ((a) == 'b') ? 2 : 10)

static ctype_status Vchar(ctype_fmt *);
static ctype_status Verr(ctype_fmt *);
static ctype_status Vflag(ctype_fmt *);
static ctype_status Vint(ctype_fmt *);
static ctype_status Vperc(ctype_fmt *);
static ctype_status Vstr(ctype_fmt *);

static uchar buf[64 * sizeof(struct fmtverb)];
ctype_arr __fmt_Fmts = c_arr_INIT(buf);

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

static i32
__get_base(char a)
{
    switch (a) {
        case 'p':
        case 'x':
            return 16;
        case 'o':
            return 8;
        case 'b':
            return 2;
        default:
            return 10;
    }
    // No way to get here?
    return 10;
}

static ctype_status
Vchar(ctype_fmt *p)
{
    ctype_rune run;
    char buf[8];

    run = va_arg(p->args, ctype_rune);
    c_mem_cpy(buf, sizeof(run), &run);
    return c_fmt_nput(p,buf, c_utf8_chartorune(&run, buf));
}

static ctype_status
Verr(ctype_fmt *p)
{
    ctype_error errnum;
    char buf[C_ERRSIZ];

    if ((errnum = va_arg(p->args, ctype_error)) == C_ECSTM)
        c_std_errstr(buf, sizeof(buf));
    else
        c_std_strerror(errnum, buf, sizeof(buf));

    return c_fmt_put(p, buf);
}

static ctype_status
Vflag(ctype_fmt *p)
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
            if (sizeof(uintptr) == sizeof(uvlong))
                p->flags |= C_FMTVLONG;
            break;
    }
    return 1;
}

static ctype_status
Vint(ctype_fmt *p)
{
    unsigned long long l = 0;
    i32 b, d, i, j, u, neg;
    char *bu;
    char buf[140];

    bu = buf + sizeof(buf) - 1;

    // Temporary positive buffer
    long long tmp_p_buf = 0;

    neg = 0;
    l = (uvlong)va_arg(p->args, uvlong);

     if (!(p->flags & C_FMTUNSIGNED)) {
        // To signed
        tmp_p_buf = *(long long*)&l;
        if (tmp_p_buf < 0)  {
            neg = 1;
            l = -(long long)l;
        }
    }

    b = __get_base(p->r);
    u = (p->r == 'X') ? 'A' : 'a';
    i = sizeof(buf) - 1;
    j = 0;

    if (!l)
        *bu-- = '0';
    
    for (; l; j++) {
        d = (l % b);
        if ((p->flags & C_FMTCOMMA) && j % 4 == 3) {
            *bu-- = ',';
            j++;
        }
        *bu-- = (d < 10) ? d + '0' : u + d - 10;
        l /= b;
    }

    if ((p->flags & C_FMTZERO) && !(p->flags & (C_FMTLEFT | C_FMTPREC))) {
        p->width -= sizeof(buf) - i;
        for (; p->width >= 0; --p->width) {
            *bu-- = '0';
        }
        p->width = 0;
    }

    if (p->flags & C_FMTSHARP) {
        if (b == 16)
            *bu-- = u + 23;
        if (b == 16 || b == 8)
            *bu-- = '0';
    }

    if (neg)
        *bu-- = '-';
    else if (p->flags & C_FMTSIGN)
        *bu-- = '+';
    else if (p->flags & C_FMTSPACE)
        *bu-- = ' ';
    
    return c_fmt_nput(p, bu + 1, sizeof(bu) - 1);
}

static ctype_status
Vperc(ctype_fmt *p)
{
    p->prec = 1;
    return c_fmt_nput(p, "%", 1);
}

static ctype_status
Vstr(ctype_fmt *p)
{
    char *s;

    s = va_arg(p->args, char *);
    return c_fmt_put(p, s);
}