set(LIBKERNEL_SOURCES
    kernel/exit.c
    kernel/write.c
    kernel/mmap.c
    kernel/fd_write.c
    internal/syscall_ret.c
    kernel/munmap.c
    kernel/open3.c
    kernel/fcntl.c
    kernel/read.c
    kernel/close.c
    kernel/stat.c
    errno/errno.c
    kernel/open2.c
    )

set(SOURCES ${LIBKERNEL_SOURCES})

covenant_static_component(libKernel_static kernel_static)

utopia_covenant_component_dynamic(libKernel Kernel)

target_link_options(libKernel PRIVATE
    "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/exported_symbols.list")

target_link_libraries(libKernel PRIVATE libmdlk)
add_dependencies(libKernel_static syscall.h)
add_dependencies(libKernel_static everytype.h)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libKernel.dylib
    PERMISSIONS OWNER_READ
    DESTINATION ${CMAKE_INSTALL_PREFIX}/Core/Libraries/covenant
)
