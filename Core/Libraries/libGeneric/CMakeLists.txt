set(LIBGENERIC_SOURCES
    hm/init.c
    internal/hm_resize.c
    internal/hm_get_hash.c
    hm/bucket/item.c
    hm/insert.c
    hm/bucket/at.c
    hm/bucket/init.c)

set(SOURCES ${LIBGENERIC_SOURCES})

covenant_static_component(libGeneric_static generic_static)