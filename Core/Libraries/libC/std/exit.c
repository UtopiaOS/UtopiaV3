

#include <covenant/std.h>

void
c_std_exit(i32 r)
{
    c_ioq_flush(ioq1);
    c_ioq_flush(ioq2);
    c_kernel_exit(r);
}