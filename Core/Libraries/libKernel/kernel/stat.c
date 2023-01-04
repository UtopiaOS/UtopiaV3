#include <covenant/kernel/basic.h>
#include <covenant/bits/syscall.h>
#include "internal/syscall.h"

// TODO: move to statx some time in the future
Status
c_kernel_stat(char* filename, Stat *stat) {
    return (Status)__syscall(SYS_statx, filename, stat);
}