

#include <covenant/std.h>

void *
c_mem_cpy(void *dest_area, USize byte_size, void *origin_area)
{
    UChar *s1, *s2;

    s1 = dest_area;
    s2 = origin_area;
    if (origin_area == dest_area)
        return dest_area;
    if (origin_area > dest_area)
    { // Expand
        for (; byte_size; byte_size--)
        {
            *s1++ = *s2++;
        }
    }
    else
    {
        s1 += byte_size - 1;
        s2 += byte_size - 1;
        for (; byte_size; byte_size--)
        {
            *s1-- = *s2--;
        }
    }
    return dest_area;
}