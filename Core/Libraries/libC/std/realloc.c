


#include <covenant/std.h>
#include "std_impl.h"

void *
c_std_realloc(void *p, usize m, usize n)
{
    return __allocator(p, m, n);
}