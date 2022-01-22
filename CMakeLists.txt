cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Meta/CMake")

project(UtopiaOS C CXX ASM)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "UtopiaOS")
    message(FATAL_ERROR "System name is not UtopiaOS, this is unsupported.\n"
                        "Please re-read the build instructions and try again!\n")
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Root)
set(CMAKE_STAGING_PREFIX ${CMAKE_BINARY_DIR}/Root)
list(APPEND CMAKE_C_SOURCE_FILE_EXTENSIONS lo)

# Create our NeoFHS file tree
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core/Libraries)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core/Binaries)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core/Headers)

add_subdirectory(Core)