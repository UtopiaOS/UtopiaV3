cmake_minimum_required(VERSION 3.16)
project(UtopiaOS C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(utopia_exe)
include(utopia_lib)

add_subdirectory(src)