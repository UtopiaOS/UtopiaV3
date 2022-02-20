

set(LIBC_SOURCES
    std/bsearch.c)

set(SOURCES ${LIBC_SOURCES})

covenant_static_component(libC_static c_static)

add_dependencies(libC_static libKernel_static)