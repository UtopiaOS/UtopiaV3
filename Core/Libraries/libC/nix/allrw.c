

#include <covenant/nix.h>

size
c_nix_allrw(ctype_iofn op, ctype_fd fd, void *p, usize n)
{
    size ret;
    uchar *s;

    s = p;
    while (n) {
        if ((ret = op(fd, p, n)) < 0)
            return ret;
        if (!ret)
            break;
        s += ret;
        n -= ret;
    }
    return s - (uchar *)p;
}