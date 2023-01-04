#include <covenant/std.h>

UniversalType c_std_malloc(USize m)
{
    return c_std_alloc(m, sizeof(Byte));
}