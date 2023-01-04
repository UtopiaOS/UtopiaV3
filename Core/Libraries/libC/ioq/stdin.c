#include <covenant/std.h>

static Byte buf[C_BIOSIZ];
static InOutObject ioq = c_ioq_INIT(0, buf, &c_kernel_fdwrite);
InOutObject *ioq0 = &ioq;