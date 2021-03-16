cmake_minimum_required(VERSION 3.16)
project(UtopiaOS C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(utopia_exe)

add_subdirectory(src)