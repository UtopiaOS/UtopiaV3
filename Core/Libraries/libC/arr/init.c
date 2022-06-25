#include <covenant/std.h>

void
c_arr_init(ctype_arr *p, char *s, usize n)
{
    p->size = n ? n - 1 : 0;
    p->length = 0;
    p->members = (uchar *)s;
}