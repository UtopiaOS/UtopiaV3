

#ifndef _COVENANT_SYS_MMAN_H
#define _COVENANT_SYS_MMAN_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size
#define __NEED_ctype_fd
#define __NEED_ctype_fssize
#define __NEED_i32

#include <covenant/bits/everytype.h>


#define C_MAP_FAILED    ((void *)-1)

#define C_MAP_SHARED            0x01
#define C_MAP_PRIVATE           0x02
#define C_MAP_SHARED_VALIDATE   0x03
#define C_MAP_TYPE              0x0f
#define C_MAP_FIXED             0x10
#define C_MAP_ANON              0x20
#define C_MAP_ANONYMOUS         C_MAP_ANON
#define C_MAP_NORESERVE         0x4000
#define C_MAP_GROWSDOWN         0x0100
#define C_MAP_DENYWRITE         0x0800
#define C_MAP_EXECUTABLE        0x1000
#define C_MAP_LOCKED            0x2000
#define C_MAP_POPULATE          0x8000
#define C_MAP_NONBLOCK          0x10000
#define C_MAP_STACK             0x20000
#define C_MAP_HUGETLB           0x40000
#define C_MAP_SYNC              0x80000
#define C_MAP_FIXED_NOREPLACE   0x100000
#define C_MAP_FILE              0

#define C_MAP_HUGE_SHIFT        26
#define C_MAP_HUGE_MASK         0x3f
#define C_MAP_HUGE_64KB         ((16 << 26))
#define C_MAP_HUGE_512KB        ((19 << 26))
#define C_MAP_HUGE_1MB          ((20 << 26))
#define C_MAP_HUGE_2MB          ((21 << 26))
#define C_MAP_HUGE_8MB          ((23 << 26))
#define C_MAP_HUGE_16MB         ((24 << 26))
#define C_MAP_HUGE_32MB         ((25 << 26))
#define C_MAP_HUGE_256MB        ((28 << 26))
#define C_MAP_HUGE_512MB        ((29 << 26))
#define C_MAP_HUGE_1GB          ((30 << 26))
#define C_MAP_HUGE_2GB          ((31 << 26))
#define C_MAP_HUGE_16GB         ((34U << 26))

#define C_PROT_NONE             0
#define C_PROT_READ             1
#define C_PROT_WRITE            2
#define C_PROT_EXEC             4
#define C_PROT_GROWSDOWN        0x01000000
#define C_PROT_GROWSUP          0x02000000

void *c_kernel_mmap(void*, size, i32, i32, ctype_fd, ctype_fssize);

#ifdef __cplusplus
"}"
#endif


#endif /* _COVENANT_SYS_MMAN_H */