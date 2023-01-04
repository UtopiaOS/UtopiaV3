#include <covenant/std.h>

USize
c_hsh_octets(HST *p)
{
    return p->len >> 3;
}