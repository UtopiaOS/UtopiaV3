cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Meta/CMake")

project(UtopiaOS C CXX ASM)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "UtopiaOS")
    message(FATAL_ERROR "System name is not UtopiaOS, this is unsupported.\n"
                        "Please re-read the build instructions and try again!\n")
endif()