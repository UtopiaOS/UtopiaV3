

#include <covenant/std.h>

usize
c_arr_avail(ctype_arr *p)
{
    return (p->size - p->length);
}