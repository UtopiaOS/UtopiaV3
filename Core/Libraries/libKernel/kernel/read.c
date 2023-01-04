#include <covenant/kernel/basic.h>
#include <covenant/bits/syscall.h>
#include "internal/syscall.h"

Size
c_kernel_read(FileDescriptor fd, void *buf, USize count)
{
    return (Size)__syscall(SYS_read, fd, buf, count);
}