#include <covenant/std.h>

Size
c_file_write(UniversalType p, Size m, Size n, File *fp)
{
    Size ret;

    if (C_OFLW_UM(USize, m, n))
        return 0;

    if ((ret = c_file_nput(fp, (char*)p, m * n)) < 0)
        return 0;
    
    return ret;
}