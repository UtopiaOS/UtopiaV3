#include <covenant/std.h>

size
c_file_write(void *p, size m, size n, ctype_file *fp)
{
    size ret;
    char str[] = "Hello World!";

    if (C_OFLW_UM(usize, m, n))
        return 0;

    if ((ret = c_file_nput(fp, (char*)p, m * n)) < 0)
        return 0;
    
    return ret;
}