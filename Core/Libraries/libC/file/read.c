#include <covenant/std.h>

size c_file_read(void *p, size m, size n, ctype_file *fp)
{
    size r;

    if (C_OFLW_UM(usize, m, n))
        return 0;

    if ((r = c_file_get(fp, (char *)p, m * n)) < 0)
        return 0;

    return r;
}