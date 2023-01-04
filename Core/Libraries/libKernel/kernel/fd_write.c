

#include <covenant/kernel/basic.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>

Size 
c_kernel_fdwrite(FileDescriptor fd, void *buf, USize count)
{
    Size res;

    do
    {
        res = c_kernel_write(fd, buf, count);
    } while ((res < 0) && errno == C_EINTR);

    return res;
}