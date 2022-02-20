set(LIBKERNEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/exit.c)

set(SOURCES ${LIBKERNEL_SOURCES})

covenant_static_component(libKernel_static kernel_static)

target_include_directories(libKernel_static PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)

add_dependencies(libKernel_static syscall.h)
add_dependencies(libKernel_static everytype.h)