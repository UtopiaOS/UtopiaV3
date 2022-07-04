#include <covenant/std.h>
#include <string.h>

void *
memset(void *s, int c, size_t n)
{
    return c_mem_set(s, n, c);
}