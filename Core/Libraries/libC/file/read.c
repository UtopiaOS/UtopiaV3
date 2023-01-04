#include <covenant/std.h>

Size c_file_read(UniversalType p, Size m, Size n, File *fp)
{
    Size r;

    if (C_OFLW_UM(USize, m, n))
        return 0;

    if ((r = c_file_get(fp, (char *)p, m * n)) < 0)
        return 0;

    return r;
}