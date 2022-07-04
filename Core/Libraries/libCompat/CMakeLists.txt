
set(LIBCOMPAT_SOURCES
    string/memset.c
)

set(SOURCES ${LIBCOMPAT_SOURCES})

utopia_library_static(libCompat_static compat_static)

add_dependencies(libCompat_static libC_static)
add_dependencies(libCompat_static alltypes.h)