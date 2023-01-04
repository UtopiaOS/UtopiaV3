


#include <covenant/std.h>
#include "std_impl.h"

UniversalType
c_std_realloc(UniversalType p, USize m, USize n)
{
    return __allocator(p, m, n);
}