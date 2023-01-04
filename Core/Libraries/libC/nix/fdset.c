#include <covenant/std.h>
#include <covenant/kernel/basic.h>
#include <covenant/status.h>

ctype_status
c_nix_fdset(FileDescriptor fd, Int32 flags)
{
    Int32 opts;

    if ((opts = c_kernel_fcntl(fd, C_KERNEL_FGETFD)) == -1)
        return -1;
    
    return c_kernel_fcntl(fd, C_KERNEL_FSETFD, opts | flags);
}