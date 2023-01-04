

#include <covenant/std.h>

#include "std_impl.h"

UniversalType
c_std_alloc(USize m, USize n)
{
    return __allocator(nil, m, n);
}