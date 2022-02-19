set(LIBKERNEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/exit.c)

# Utopia syscall interface generation
add_custom_target(syscall.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/covenant/bits
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/covenant/bits/syscall.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/covenant/bits/syscall.h
    COMMAND sed -n -e s/__LNX_/SYS_/p < ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/covenant/bits/syscall.h.in
        >> ${CMAKE_CURRENT_BINARY_DIR}/include/covenant/bits/syscall.h
    COMMENT include/bits/syscall.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/covenant/bits/syscall.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/covenant/bits/syscall.h.in
)

set(SOURCES ${LIBKERNEL_SOURCES})

covenant_static_component(libKernel_static kernel_static)

target_include_directories(libKernel_static PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)

add_dependencies(libKernel_static syscall.h)
