#include <covenant/kernel/basic.h>
#include <covenant/bits/syscall.h>
#include "internal/syscall.h"

// TODO: Should we really handle it like this, is the type casting correct
ctype_status
c_kernel_close(ctype_fd fd) {
    return (ctype_status)__syscall(SYS_close, fd);
}