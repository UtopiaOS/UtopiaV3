set(LIBKERNEL_SOURCES
    kernel/exit.c
    kernel/write.c
    kernel/mmap.c
    kernel/fd_write.c
    internal/syscall_ret.c
    kernel/munmap.c
    errno/errno.c)

set(SOURCES ${LIBKERNEL_SOURCES})

covenant_static_component(libKernel_static kernel_static)

utopia_covenant_component_dynamic(libKernel kernel)
add_dependencies(libKernel_static syscall.h)
add_dependencies(libKernel_static everytype.h)