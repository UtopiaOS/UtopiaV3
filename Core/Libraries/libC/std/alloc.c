

#include <covenant/std.h>

#include "std_impl.h"

void *
c_std_alloc(usize m, usize n)
{
    return __allocator(nil, m, n);
}