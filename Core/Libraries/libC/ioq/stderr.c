#include <covenant/std.h>

static Byte buf[C_ERRSIZ];
static InOutObject ioq = c_ioq_INIT(2, buf, &c_kernel_fdwrite);
InOutObject *ioq2 = &ioq;