

add_executable(
    mldk
    main.c
)

target_link_libraries(mldk PUBLIC libKernel_static libC_static libGeneric_static)

target_link_libraries(
    mldk
    INTERFACE  
    libKernel_headers
    libC_headers
)

target_compile_options(mldk PRIVATE
-ffreestanding
-fno-stack-protector)

target_link_options(mldk PRIVATE
    -Wl,-dylinker
    -Wl,-dylinker_install_name,/Core/Binaries/mdlk
    -fvisibility=hidden

    -Wl,-segaddr,__TEXT,0x1000
)

set_property(TARGET mldk PROPERTY POSITION_INDEPENDENT_CODE ON)

add_dependencies(mldk libC_static)

