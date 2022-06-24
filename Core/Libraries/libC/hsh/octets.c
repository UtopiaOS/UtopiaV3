#include <covenant/std.h>

usize
c_hsh_octets(ctype_hst *p)
{
    return p->len >> 3;
}