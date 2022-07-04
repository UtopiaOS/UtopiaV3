cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Meta/CMake")

project(UtopiaOS C CXX ASM)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "UtopiaOS")
    message(FATAL_ERROR "System name is not UtopiaOS, this is unsupported.\n"
                        "Please re-read the build instructions and try again!\n")
endif()

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Root)
set(CMAKE_STAGING_PREFIX ${CMAKE_BINARY_DIR}/Root)
list(APPEND CMAKE_C_SOURCE_FILE_EXTENSIONS lo)

# Kill default /usr/include/* dirs...
unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)

set(PROJECT_META_DIR ${PROJECT_SOURCE_DIR}/Meta)

# Headers that are used during the build process, some might be installed
set(UNIVERSAL_BUILD_UTOPIA_HEADERS ${PROJECT_SOURCE_DIR}/Core/Headers)

# Create our NeoFHS file tree
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core/Libraries)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core/Binaries)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/Core/Headers)

include(covenant)
include(utils)
include(libcompat)

list(APPEND COVENANT_COMPONENTS "libKernel" "libC" "libGeneric")

covenant_bootstrap_all_include_directories("${COVENANT_COMPONENTS}")
# TODO: Unhardcode this
include_directories(Core/Support/mdlk)
include_directories(Core/Support/mdlk/include)
include_directories(Core/Libraries/libMacho/include)
include_directories(Core/Libraries/libCompat/include)


covenant_configure_syscalls()
covenant_configure_types()
libcompat_configure_types()

# Configure include directories
include_directories(.)
include_directories(Core/Headers)
include_directories(Core/Libraries)
include_directories(Core/Libraries/libKernel)
include_directories(Core/Libraries/libC)
include_directories(Core/Libraries/libCompat)
include_directories(Core/Libraries/libGeneric)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Core/Headers)


add_subdirectory(Core)