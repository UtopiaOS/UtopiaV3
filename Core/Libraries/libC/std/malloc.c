#include <covenant/std.h>

void *c_std_malloc(usize m)
{
    return c_std_alloc(m, sizeof(uchar));
}