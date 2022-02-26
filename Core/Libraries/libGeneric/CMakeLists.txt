set(LIBGENERIC_SOURCES
    hm/init.c
    hm/bucket/at.c
    hm/bucket/init.c)

set(SOURCES ${LIBGENERIC_SOURCES})

covenant_static_component(libGeneric_static generic_static)