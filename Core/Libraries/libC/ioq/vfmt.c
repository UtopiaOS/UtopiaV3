

#include <covenant/std.h>

static Status
put(Format *p, char *s, USize n)
{
    return c_ioq_nput(p->farg, s, n);
}

Size
c_ioq_vfmt(InOutObject *p, char *fmt, VaList args)
{
    Format f;

    c_fmt_init(&f, p, put);
    va_copy(f.args, args);
    return c_fmt_fmt(&f, fmt);
}