

set(LIBC_SOURCES
    std/bsearch.c
    std/pubrealloc.c
    std/realloc.c
    std/alloc.c
    std/setalloc.c
    arr/avail.c
    dyn/ready.c
    nix/munmap.c)

set(SOURCES ${LIBC_SOURCES})

covenant_static_component(libC_static c_static)

add_dependencies(libC_static libKernel_static)