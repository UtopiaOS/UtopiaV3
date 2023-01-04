
#include <covenant/std.h>
#include <covenant/nix.h>

#define BLK(x) C_MIN(C_BIOSIZ, (x))

Status
c_ioq_nput(InOutObject *p, char *s, USize n)
{
    Size ret;

    if (n > c_arr_avail(&p->array)) {
        if (c_ioq_flush(p) < 0)
            return -1;
        while (n > c_arr_avail(&p->array)) {
            if ((ret = c_nix_allrw(p->op, p->fd, s, BLK(n))) < 0)
                return -1;
            n -= ret;
            s += ret;
        }
    }
    c_mem_cpy(p->array.members + p->array.capacity, n, s);
    p->array.capacity += n;
    return 0;
}