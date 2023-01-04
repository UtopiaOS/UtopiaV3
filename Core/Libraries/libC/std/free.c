

#include <covenant/std.h>
#include "internal/std_impl.h"

UniversalType
c_std_free_(UniversalType p)
{
    return __allocator(p, 0, sizeof(Byte));
}