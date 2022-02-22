

#include <covenant/std.h>
#include <covenant/shared.h>

void
c_str_errstr(char *str_err, usize size)
{
    usize i;
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
