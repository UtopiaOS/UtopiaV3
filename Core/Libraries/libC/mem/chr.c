#include <covenant/std.h>
#include <covenant/limits.h>

#define SS (sizeof(Size))
#define ALIGN (sizeof(Size)-1)
#define ONES ((Size)-1/C_UCHAR_MAX)
#define HIGHS (ONES * (C_UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *
c_mem_chr(void *src, Size n, Int32 c)
{
    UChar *s;

    s = src;
    for (; n; --n) {
        if (*s == c)
            return s;
        ++s;
    }
    return nil;
}