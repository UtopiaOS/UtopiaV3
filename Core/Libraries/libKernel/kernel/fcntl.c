#include <covenant/kernel/basic.h>
#include "internal/syscall.h"

i32
c_kernel_fcntl(ctype_fd fd, i32 cmd, ...) {
    ulong arg;
    va_list ap;

    va_start(ap, cmd);
    arg = va_arg(ap, ulong);
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