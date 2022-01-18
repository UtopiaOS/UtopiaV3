
if(__UTOPIA_PATHS_INCLUDED)
    return()
endif()
set(__UTOPIA_PATHS_INCLUDED)

set(UTOPIA 1)

get_filename_component(_CMAKE_INSTALL_DIR "${CMAKE_ROOT}" PATH)
get_filename_component(_CMAKE_INSTALL_DIR "${_CMAKE_INSTALL_DIR}" PATH)

list(APPEND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
    /Core/Libraries
    /System/Index/lib
)
