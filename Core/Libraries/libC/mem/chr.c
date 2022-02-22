#include <covenant/std.h>
#include <covenant/limits.h>

#define SS (sizeof(size))
#define ALIGN (sizeof(size)-1)
#define ONES ((size)-1/C_UCHAR_MAX)
#define HIGHS (ONES * (C_UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *
c_mem_chr(const void *src, size n, i32 c)
{
    const uchar *s = src;
    c = (uchar)c;
    #ifdef __GNUC__
        for(; ((uintptr)s & ALIGN) && n && *s != c; s++, n--);
        if (n && *s != c) {
            typedef size __attribute__((__may_alias__)) word;
            const word *w;
            size k = ONES * c;
            for (w = (const void *)s; n >= SS && !HASZERO(*w^k); w++, n-=SS);
            s = (const void *)w;
        }
    #endif /* __GNUC__ */
    for (; n && *s != c; s++, n--);
    return n ? (void *)s : 0;
}