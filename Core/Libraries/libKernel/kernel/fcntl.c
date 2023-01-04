#include <covenant/kernel/basic.h>
#include "internal/syscall.h"

Int32
c_kernel_fcntl(FileDescriptor fd, Int32 cmd, ...) {
    ULong arg;
    VaList ap;

    va_start(ap, cmd);
    arg = va_arg(ap, ULong);
    va_end(ap);
    switch(cmd) {
        case C_KERNEL_FGETLK:
        case C_KERNEL_FSETLK:
        case C_KERNEL_FSETLKW:
            return __syscall(SYS_fcntl, fd, cmd, (void*)arg);
        default:
            return __syscall(SYS_fcntl, fd, cmd, arg);
    }
}