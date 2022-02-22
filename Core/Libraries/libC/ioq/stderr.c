#include <covenant/std.h>

static uchar buf[C_ERRSIZ];
static ctype_ioq ioq = c_ioq_INIT(2, buf, &c_kernel_fdwrite);
ctype_ioq *ioq2 = &ioq;