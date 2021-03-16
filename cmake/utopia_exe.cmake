FUNCTION(add_utopia_binary bin)
    foreach(file in LIST ARGN)
        set(files ${files} ${file})
    endforeach(file)

    add_executable(${bin} ${files})
ENDFUNCTION(add_utopia_binary)