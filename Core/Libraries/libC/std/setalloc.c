#include <covenant/std.h>

#include "std_impl.h"

AllocationFunction __allocator = pubrealloc;

void
c_std_setalloc(AllocationFunction f)
{
    __allocator = f ? f : pubrealloc;
}