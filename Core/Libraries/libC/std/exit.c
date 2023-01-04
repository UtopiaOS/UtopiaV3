

#include <covenant/std.h>

// TODO: This might be moved to status in the future
Void
c_std_exit(Int32 r)
{
    c_ioq_flush(ioq1);
    c_ioq_flush(ioq2);
    c_kernel_exit(r);
}