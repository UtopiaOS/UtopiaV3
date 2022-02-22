

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
    std/errno.c
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
    fmt/put.c
    nix/munmap.c)

set(SOURCES ${LIBC_SOURCES})

covenant_static_component(libC_static c_static)

add_dependencies(libC_static libKernel_static)