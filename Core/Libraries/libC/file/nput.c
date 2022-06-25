#include <covenant/std.h>

#define BLK(x) C_MIN(C_BIOSIZ, (x))

ctype_status
c_file_nput(ctype_file *fp, char *s, usize n)
{
    size ret;

    if (n > c_arr_avail(&fp->data)) {
        if (c_file_flush(fp) < 0)
            return -1;
        while (n > c_arr_avail(&fp->data)) {
            if ((ret = c_nix_allrw(fp->writefn, fp->fd, s, BLK(n))) < 0)
                return -1;
            n -= ret;
            s += ret;
        }
    }

    c_mem_cpy(fp->data.members + fp->data.length, n, s);
    fp->data.length += n;

    return 0;
}