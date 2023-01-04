

#include <covenant/std.h>
#include <covenant/shared.h>

Void
c_std_errstr(char *str_err, USize size)
{
    USize i;
    char *p;

    p = c_std_getsyserr();
    size = C_MIN(size, C_ERRSIZ);
    for (i = 0; i < size; i++) {
        str_err[i] ^= p[i];
        p[i] ^= str_err[i];
        str_err[i] ^= p[i];
    }
    errno = C_ECSTM;
}
