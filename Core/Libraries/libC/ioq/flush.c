
#include <covenant/std.h>
#include <covenant/nix.h>

ctype_status
c_ioq_flush(ctype_ioq *p)
{
    size ret;

    if ((ret = c_nix_allrw(p->op, p->fd, p->array.members, p->array.length)) < 0)
        return -1;
    
    c_arr_trunc(&p->array, p->array.length - ret, sizeof(uchar));
    return 0;
}