
#include <covenant/std.h>

// Set what, of what size to what value.
void *
c_mem_set(void *v, usize n, i32 c)
{
    uchar *s;

    s = v;
    for (; n; n--) {
        *s++ = c;
    }

    __asm__ __volatile__(""::"r"(v):"memory");
    return v;
}