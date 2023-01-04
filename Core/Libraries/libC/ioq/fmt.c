#include <covenant/std.h>

Size
c_ioq_fmt(InOutObject *p, char *fmt, ...)
{
    VaList ap;
    Size n;

    va_start(ap, fmt);
    n = c_ioq_vfmt(p, fmt, ap);
    va_end(ap);
    return n;
}