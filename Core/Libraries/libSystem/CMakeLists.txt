
set(STATIC_LIB_COVENANT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/exit.c
)

set(COMMON_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/generic
)


add_library(covenant_static STATIC ${STATIC_LIB_COVENANT_SOURCES})

target_include_directories(covenant_static PRIVATE ${COMMON_INCLUDE_DIRECTORIES})



add_dependencies(covenant_static syscall.h saint_alliance.h)

# Utopia's saint alliance
add_custom_target(saint_alliance.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/bits
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/include/saint_alliance.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/bits/saint_alliance.h
    COMMENT include/bits/saint_alliance.h
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/newixtypes.h
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/alltypes.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/saint_alliance.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/saint_alliance.h.in
)


# Utopia subset of files
add_custom_target(newixtypes.h
    COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/include/bits
    COMMAND sed -f ${PROJECT_META_DIR}/sed/mkalltypes.sed
        ${CMAKE_CURRENT_SOURCE_DIR}/include/newixtypes.h.in
        > ${CMAKE_CURRENT_BINARY_DIR}/include/bits/newixtypes.h
    COMMENT include/bits/newixtypes.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/newixtypes.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/newixtypes.h.in
)

# Utopia POSIX-like/UNIX like types and struct generataion
add_custom_target(alltypes.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/bits
    COMMAND sed -f ${PROJECT_META_DIR}/sed/mkalltypes.sed
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/alltypes.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/alltypes.h.in
        > ${CMAKE_CURRENT_BINARY_DIR}/include/bits/alltypes.h
    COMMENT include/bits/alltypes.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/alltypes.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/alltypes.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/alltypes.h.in)


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