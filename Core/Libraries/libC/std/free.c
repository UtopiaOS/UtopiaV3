

#include <covenant/std.h>
#include "internal/std_impl.h"

void*
c_std_free_(void *p)
{
    return __allocator(p, 0, sizeof(uchar));
}