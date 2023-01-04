

#include <covenant/nix.h>

Size
c_nix_allrw(IOFunction op, FileDescriptor fd, void *p, USize n)
{
    Size ret;
    UChar *s;

    s = p;
    while (n) {
        if ((ret = op(fd, p, n)) < 0)
            return ret;
        if (!ret)
            break;
        s += ret;
        n -= ret;
    }
    return s - (UChar *)p;
}