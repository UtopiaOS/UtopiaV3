
#include <covenant/std.h>
#include <covenant/nix.h>

Status
c_ioq_flush(InOutObject *p)
{
    Size ret;

    if ((ret = c_nix_allrw(p->op, p->fd, p->array.members, p->array.capacity)) < 0)
        return -1;
    
    c_arr_trunc(&p->array, p->array.capacity - ret, sizeof(Byte));
    return 0;
}