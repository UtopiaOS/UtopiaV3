

#include <covenant/errcode.h>
#include "syscall.h"
#include <covenant/shared.h>

// TODO: Rewrite with ULong
long 
__syscall_ret(unsigned long r)
{
    if (r > -4096UL) {
        errno = -r;
    }
    return r;
}
