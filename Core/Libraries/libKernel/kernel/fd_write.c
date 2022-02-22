

#include <covenant/kernel/basic.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>

size 
c_kernel_fdwrite(ctype_fd fd, void *buf, usize count)
{
    size res;

    do
    {
        res = c_kernel_write(fd, buf, count);
    } while ((res < 0) && errno == C_EINTR);

    return res;
}