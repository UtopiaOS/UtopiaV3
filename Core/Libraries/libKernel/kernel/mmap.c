

#include <covenant/kernel/basic.h>
#include <covenant/shared.h>
#include <covenant/errcode.h>
#include <covenant/sys/mman.h>
#include <covenant/int.h>
#include "syscall.h"

#define UNIT SYSCALL_MMAP2_UNIT
#define KERNEL_OFF_MASK ((-0x2000ULL << (8*sizeof(syscall_arg_t)-1)) | (UNIT-1))

static void __vm_wait(void) { };

void*
c_kernel_mmap(void *start, size len, u32 prot, u32 flags, ctype_fd fd, ctype_fssize off) 
{
    long ret;
    if (off & KERNEL_OFF_MASK){
        errno = C_EINVAL;
        return C_MAP_FAILED;
    }
    if (len >= PTRDIFF_MAX) {
        errno = C_ENOMEM;
        return C_MAP_FAILED;
    }
    if (flags & MAP_FIXED) {
        __vm_wait();
    }
    #ifdef SYS_mmap2
        ret = __syscall(SYS_mmap2, start, len, prot, flags, fd, off/UNIT);
    #else
        ret = __syscall(SYS_mmap, start, len, prot, flags, fd, off);
    #endif
    /* Fixup incorrect EPERM from kernel */
    if (ret == -C_EPERM && !start && (flags&C_MAP_ANON) && !(flags&C_MAP_FIXED))
        ret = -C_ENOMEM;
    return (void*)__syscall_ret(ret);
}