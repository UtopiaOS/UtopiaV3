#include <stdlib.h>
#include <stdmem.h>

void *
c_mem_chr(void *v, usize n, u32 c)
{
    uchar *s;

    s = v;
    for (; n; n--) {
        if (*s == c)
            return s;
        ++s;
    }
    return nil;
}