
utopia_component_interface(libMacho)

set(MACHO_DUMMY
    main.c)

set(SOURCES ${MACHO_DUMMY})

utopia_dummy_component(macho_dummy)
add_dependencies(libMacho libC)
add_dependencies(libMacho libKernel)
target_link_libraries(macho_dummy PUBLIC libMacho)