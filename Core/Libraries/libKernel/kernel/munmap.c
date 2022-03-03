
#include <covenant/kernel/basic.h>
#include "syscall.h"

static void __vm_wait(void) { }

ctype_status 
c_kernel_munmap(void *start, size len)
{
    __vm_wait();
    return (ctype_status)syscall(SYS_munmap, start, len);
}