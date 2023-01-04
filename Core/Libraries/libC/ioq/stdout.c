#include <covenant/std.h>

static Byte buf[C_BIOSIZ];
static InOutObject ioq = c_ioq_INIT(1, buf, &c_kernel_fdwrite);
InOutObject *ioq1 = &ioq;