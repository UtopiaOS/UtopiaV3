#include <covenant/std.h>

static Status
put(Format *format, char *s, USize n) {
    return c_arr_push(format->farg, s, n, sizeof(Byte));
}

Size
c_arr_vfmt(Array *array, char *fmt, VaList args) {
    Format f;
    c_fmt_init(&f, array, put);

    va_copy(f.args, args);
    return c_fmt_fmt(&f, fmt);
}