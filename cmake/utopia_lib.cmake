FUNCTION(add_utopia_library name)
    foreach(file IN LISTS ARGN)
        set(files ${files} ${f})
    endforeach(f)

    set(CMAKE_SKIP_RPATH TRUE)
    add_library(${name} SHARED ${files})

    set_target_properties(${name} PROPERTIES PREFIX "lib")

ENDFUNCTION(add_utopia_library)