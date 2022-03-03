

add_executable(
    mldk
    main.c
)

target_link_libraries(mldk PUBLIC libKernel_static libC_static)

target_link_libraries(
    mldk
    INTERFACE  
    libKernel_headers
    libC_headers
)

target_compile_options(mldk PRIVATE
-ffreestanding)

target_link_options(mldk PUBLIC
    -Wl,-static)

set_property(TARGET mldk PROPERTY POSITION_INDEPENDENT_CODE ON)

add_dependencies(mldk libC_static)

