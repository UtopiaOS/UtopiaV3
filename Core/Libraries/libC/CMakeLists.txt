

set(LIBC_SOURCES
    std/bsearch.c
    std/pubrealloc.c
    std/realloc.c
    std/alloc.c
    std/malloc.c
    std/setalloc.c
    arr/data.c
    arr/len.c
    arr/avail.c
    dyn/ready.c
    dyn/cat.c
    arr/bytes.c
    dyn/alloc.c
    dyn/idxcat.c
    fmt/fmt.c
    fmt/init.c
    fmt/install.c
    nix/allrw.c
    arr/trunc.c
    ioq/flush.c
    ioq/nput.c
    ioq/vfmt.c
    ioq/fmt.c
    std/exit.c
    ioq/stderr.c
    ioq/stdin.c
    ioq/stdout.c
    mem/cpy.c
    fmt/nput.c
    utf8/checkrune.c
    mem/chr.c
    utf8/mtab.c
    utf8/charntorune.c
    utf8/chartorune.c
    str/len.c
    std/getsyserr.c
    fmt/put.c
    std/strerror.c
    std/errstr.c
    fmt/verb.c
    std/free.c
    mem/set.c
    str/cpy.c
    str/cmp.c
    hsh/update.c
    hsh/murmur3.c
    std/nbsearch.c
    str/pncpy.c
    str/ncpy.c
    nix/munmap.c
    u32/unpack.c
    hsh/rol32.c
    u32/pack.c
    hsh/octets.c
    hsh/murmur32.c)

#if(${UTOPIA_ARCH} STREQUAL "x86_64")
#   list(APPEND ${LIBC_SOURCES} mem/set.s)
#endif()

set(SOURCES ${LIBC_SOURCES})

covenant_static_component(libC_static c_static)

utopia_covenant_component_dynamic(libC C)
target_link_options(libC PRIVATE
    -Wl,-reexported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/reexported_symbols.list)
target_link_libraries(libC PRIVATE libKernel)
add_dependencies(libC libKernel)
add_dependencies(libC_static libKernel_static)