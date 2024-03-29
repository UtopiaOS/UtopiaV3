#include <covenant/std.h>

// Read from an ioq, into a b, of a max size of n
Size c_ioq_get(InOutObject *p, char *b, USize n)
{
    USize len, min;
    Size r;

    len = n;
    while (len)
    {
        if ((r = c_ioq_feed(p)) < 0)
            return -1; // Failed to feed
        if (!r)
            break;

        min = C_MIN((USize)r, len);
        c_mem_cpy(b, min, p->array.members + p->array.size);
        p->array.capacity -= min;
        p->array.size += min;
        len -= min;
        b += min;
    }

    return len ? n - len : n;
}