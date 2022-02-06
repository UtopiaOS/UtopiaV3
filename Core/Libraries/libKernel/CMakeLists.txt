set(LIBKERNEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/exit.c)

set(LIBKERNEL_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/generic
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/internal
    ${UNIVERSAL_BUILD_UTOPIA_HEADERS})

add_library(kernel_static STATIC ${LIBKERNEL_SOURCES})
target_include_directories(kernel_static PRIVATE ${LIBKERNEL_INCLUDE_DIRECTORIES})

add_dependencies(kernel_static syscall.h)

# Utopia syscall interface generation
add_custom_target(syscall.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/bits
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    COMMAND sed -n -e s/__LNX_/SYS_/p < ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
        >> ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    COMMENT include/bits/syscall.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
)