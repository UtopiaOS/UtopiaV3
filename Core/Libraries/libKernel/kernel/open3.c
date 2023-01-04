#include <covenant/kernel/basic.h>
#include "internal/syscall.h"

Int32
c_kernel_open3(char *s, Int32 opts, UInt32 mode)
{
    return (Int32)sys_open(s, opts, mode);
}