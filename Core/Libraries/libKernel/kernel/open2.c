#include <covenant/kernel/basic.h>
#include "internal/syscall.h"

// TODO: This actually returns a file descriptor
Int32
c_kernel_open2(char* s, Int32 opts) {
    return (Int32)sys_open(s, opts);
}
