#include <covenant/std.h>
#include <covenant/limits.h>

#define SS (sizeof(size))
#define ALIGN (sizeof(size)-1)
#define ONES ((size)-1/C_UCHAR_MAX)
#define HIGHS (ONES * (C_UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *
c_mem_chr(void *src, size n, i32 c)
{
    uchar *s;

    s = src;
    for (; n; --n) {
        if (*s == c)
            return s;
        ++s;
    }
    return nil;
}