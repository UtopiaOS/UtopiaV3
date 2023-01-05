set(LIBGENERIC_SOURCES
    hm/init.c
    internal/hm_resize.c
    internal/hm_get_hash.c
    hm/bucket/item.c
    hm/get.c
    hm/insert.c
    hm/bucket/at.c
    hm/bucket/init.c
    tq/add.c
    tq/getn.c
    tq/getnat.c
    tq/getv.c
    tq/getvat.c
    tq/init.c
    tq/insert.c
    tq/pop_head.c
    tq/pop_tail.c
    tq/push.c
    tq/removeat.c
    tq/node/init.c
    )

set(SOURCES ${LIBGENERIC_SOURCES})

covenant_static_component(libGeneric_static generic_static)
