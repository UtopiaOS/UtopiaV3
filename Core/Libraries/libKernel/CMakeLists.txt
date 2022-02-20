set(LIBKERNEL_SOURCES
    kernel/exit.c
    kernel/write.c
    kernel/mmap.c
    kernel/fd_write.c
    kernel/munmap.c)

set(SOURCES ${LIBKERNEL_SOURCES})

covenant_static_component(libKernel_static kernel_static)

add_dependencies(libKernel_static syscall.h)
add_dependencies(libKernel_static everytype.h)