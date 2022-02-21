

set(LIBC_SOURCES
    std/bsearch.c
    std/pubrealloc.c
    std/realloc.c
    std/alloc.c
    std/setalloc.c
    arr/data.c
    arr/len.c
    arr/avail.c
    dyn/ready.c
    dyn/cat.c
    arr/bytes.c
    dyn/alloc.c
    dyn/idxcat.c
    fmt/install.c
    nix/munmap.c)

set(SOURCES ${LIBC_SOURCES})

covenant_static_component(libC_static c_static)

add_dependencies(libC_static libKernel_static)