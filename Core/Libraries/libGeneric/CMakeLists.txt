set(LIBGENERIC_SOURCES
    hm/init.c
    internal/hm_resize.c
    internal/hm_get_hash.c
    internal/vec_assign.c
    internal/vec_grow.c
    internal/vec_move_left.c
    internal/vec_move_right.c
    internal/vec_offset.c
    internal/vec_realloc.c
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
    vec/assign.c
    vec/back.c
    vec/clear.c
    vec/erase.c
    vec/front.c
    vec/get.c
    vec/init.c
    vec/insert.c
    vec/pop_back.c
    vec/pop_front.c
    vec/push_back.c
    vec/resize.c
    )

set(SOURCES ${LIBGENERIC_SOURCES})

covenant_static_component(libGeneric_static generic_static)
