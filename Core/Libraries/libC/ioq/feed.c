#include <covenant/std.h>

// Feed the contents of a certain fd to the internal array.
size
c_ioq_feed(ctype_ioq *p)
{
    size r;

    if (p->array.length)
        return p->array.length; // We already have data


    if ((r = (p->op)(p->fd, p->array.members, p->array.size)) <= 0)
        return r;

    p->array.length = r;
    p->array.size -= r;
    if (p->array.size) {
        c_mem_cpy(p->array.members + p->array.size, r, p->array.members);
    }

    return r;
}