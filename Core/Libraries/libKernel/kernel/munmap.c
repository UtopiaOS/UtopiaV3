
#include <covenant/kernel/basic.h>
#include <covenant/status.h>
#include "syscall.h"

static void __vm_wait(void) { }

Status 
c_kernel_munmap(void *start, Size len)
{
    __vm_wait();
    return (Status)syscall(SYS_munmap, start, len);
}