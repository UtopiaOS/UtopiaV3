



# Utopia POSIX-like/UNIX like types and struct generataion
add_custom_target(alltypes.h
    COMMAND mdkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/bits
    COMMAND sed -f ${PROJECT_META_DIR}/mkalltypes.sed
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
    COMMAND sed -n -e s|__LNX_|SYS_|p < ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
        >> ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    COMMENT include/bits/syscall.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/bits/syscall.h
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/${UTOPIA_ARCH}/bits/syscall.h.in
)