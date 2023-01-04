#include <covenant/std.h>

USize
c_hsh_octets(ctype_hst *p)
{
    return p->len >> 3;
}