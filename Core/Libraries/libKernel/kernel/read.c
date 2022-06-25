#include <covenant/kernel/basic.h>
#include <covenant/bits/syscall.h>
#include "internal/syscall.h"

size
c_kernel_read(ctype_fd fd, void *buf, usize count)
{
    return (size)__syscall(SYS_read, fd, buf, count);
}