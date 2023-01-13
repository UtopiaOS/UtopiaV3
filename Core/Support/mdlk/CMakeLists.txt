

add_executable(
    mdlk
    mdlk.c
    ptrkey.c
    initialization.c
    relocations.c
    mdlk_startup.S
)

target_link_libraries(mdlk PRIVATE libKernel_static libC_static libGeneric_static libCompat_static)

target_link_libraries(
    mdlk
    INTERFACE
    libKernel_headers
    libC_headers
)

target_compile_options(mdlk PRIVATE
-ffreestanding
-fno-stack-protector)

target_link_options(mdlk PRIVATE
    -Wl,-dylinker
    -Wl,-dylinker_install_name,/Core/Support/mdlk
    -fvisibility=hidden

    -Wl,-segaddr,__TEXT,0x1000
)

set_property(TARGET mdlk PROPERTY POSITION_INDEPENDENT_CODE ON)

set(SOURCES 
    stub/stubs.S
    ${CMAKE_SOURCE_DIR}/Meta/Filler/dummy.c)

utopia_support_component_dynamic(libmdlk mdlk)

add_dependencies(mdlk libC_static)
add_dependencies(mdlk libCompat_static)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/mdlk
    PERMISSIONS OWNER_EXECUTE OWNER_READ
    DESTINATION ${CMAKE_INSTALL_PREFIX}/Core/Support
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libmdlk.dylib
    PERMISSIONS OWNER_EXECUTE OWNER_READ
    DESTINATION ${CMAKE_INSTALL_PREFIX}/Core/Support
)
