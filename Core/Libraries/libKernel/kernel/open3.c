#include <covenant/kernel/basic.h>
#include "internal/syscall.h"

i32
c_kernel_open3(char *s, i32 opts, u32 mode)
{
    return (i32)sys_open(s, opts, mode);
}