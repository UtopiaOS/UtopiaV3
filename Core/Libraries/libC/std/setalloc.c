#include <covenant/std.h>

#include "std_impl.h"

ctype_allocfn __allocator = pubrealloc;

void
c_std_setalloc(ctype_allocfn f)
{
    __allocator = f ? f : pubrealloc;
}