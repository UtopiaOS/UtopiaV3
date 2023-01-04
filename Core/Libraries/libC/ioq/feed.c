#include <covenant/std.h>

// Feed the contents of a certain fd to the internal array.
Size
c_ioq_feed(InOutObject *p)
{
    Size r;

    if (p->array.capacity)
        return p->array.capacity; // We already have data


    if ((r = (p->op)(p->fd, p->array.members, p->array.size)) <= 0)
        return r;

    p->array.capacity = r;
    p->array.size -= r;
    if (p->array.size) {
        c_mem_cpy(p->array.members + p->array.size, r, p->array.members);
    }

    return r;
}