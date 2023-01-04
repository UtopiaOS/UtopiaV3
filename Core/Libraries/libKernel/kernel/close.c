#include <covenant/kernel/basic.h>
#include <covenant/bits/syscall.h>
#include <covenant/status.h>
#include "internal/syscall.h"

// TODO: Should we really handle it like this, is the type casting correct
Status
c_kernel_close(FileDescriptor fd) {
    return (Status)__syscall(SYS_close, fd);
}